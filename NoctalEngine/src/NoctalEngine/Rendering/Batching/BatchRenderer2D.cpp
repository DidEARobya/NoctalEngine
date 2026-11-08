#include "nepch.h"
#include "BatchRenderer2D.h"
#include "NoctalEngine/Rendering/WrappedRenderer.h"
#include "NoctalEngine/Rendering/Drawables/Drawable.h"
#include "NoctalEngine/Rendering/Buffers/VertexBuffer.h"
#include "NoctalEngine/Rendering/Buffers/IndexBuffer.h"
#include "NoctalEngine/Rendering/Buffers/ConstantBuffer.h"
#include "NoctalEngine/Rendering/Pipeline/Pipeline.h"
#include "NoctalEngine/Rendering/Textures/Texture.h"
#include "NoctalEngine/Rendering/Shaders/Shader.h"

namespace NoctalEngine
{
	// UVs for immediate-mode quads, in the same corner order as QuadDrawable.
	static const std::array<glm::vec2, 4> s_QuadTexCoords =
	{
		glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f)
	};

	BatchRenderer2D::BatchRenderer2D(WrappedRenderer& backend) : m_Backend(backend)
	{
	}

	BatchRenderer2D::~BatchRenderer2D() = default;

	void BatchRenderer2D::Init()
	{
		// ---- White texture (slot 0) ----
		// Untextured shapes sample this, so the shader never needs an "is textured?" branch:
		// white * colour = colour.
		const uint32_t whitePixel = 0xFFFFFFFF;
		m_WhiteTexture = m_Backend.CreateTexture(1, 1, &whitePixel);

		// ---- Vertex buffer ----
		// The 'Name' of each element is the semantic the HLSL vertex shader uses.
		BufferLayout layout =
		{
			{ ShaderDataType::POS_3D,  "POSITION" },
			{ ShaderDataType::FLOAT_4, "COLOR"    },
			{ ShaderDataType::POS_2D,  "TEXCOORD" },
			{ ShaderDataType::INT,     "TEXINDEX" },
		};

		NE_ENGINE_ASSERT(layout.GetStride() == sizeof(BatchVertex), "BufferLayout doesn't match BatchVertex");

		m_VertexBuffer = m_Backend.CreateVertexBuffer(RendererConfig::MaxQuadsPerFrame * 4 * sizeof(BatchVertex), layout);

		// ---- Index buffer ----
		// Every quad uses the same pattern, just offset by 4 vertices each time:
		//   3---2
		//   | / |     triangle 1 = 0,1,2   triangle 2 = 2,3,0
		//   0---1
		// So we build it once for the biggest possible batch and never touch it again.
		std::vector<uint16_t> indices(RendererConfig::MaxIndicesPerBatch);
		uint16_t offset = 0;

		for (uint32_t i = 0; i < RendererConfig::MaxIndicesPerBatch; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		m_IndexBuffer = m_Backend.CreateIndexBuffer(indices.data(), (uint32_t)indices.size(), IndexFormat::UINT16);

		// ---- Camera constant buffer (register b0) ----
		m_CameraBuffer = m_Backend.CreateConstantBuffer(sizeof(glm::mat4), 0);

		// ---- Pipeline ----
		// Each backend loads its own version of these shaders, under the same names.
		PipelineSpecification spec;
		spec.VertexShader = m_Backend.GetShader("Batch2DVS");
		spec.PixelShader = m_Backend.GetShader("Batch2DPS");
		spec.Layout = layout;
		spec.AlphaBlending = true;
		spec.DebugName = "Batch2D";

		m_Pipeline = m_Backend.CreatePipeline(spec);
		NE_ENGINE_ASSERT(m_Pipeline && m_Pipeline->IsValid(), "Failed to create Batch2D pipeline");

		// Allocate the CPU vertex array once. Flush() only ever writes into it.
		m_Vertices.resize(RendererConfig::MaxVerticesPerBatch);
		m_TextureSlots[0] = m_WhiteTexture.get();
	}

	void BatchRenderer2D::Shutdown()
	{
		m_Pipeline.reset();
		m_CameraBuffer.reset();
		m_IndexBuffer.reset();
		m_VertexBuffer.reset();
		m_WhiteTexture.reset();
		m_TextureSlots.fill(nullptr);
	}

	void BatchRenderer2D::SubmitQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& colour, Texture* texture, int32_t layer)
	{
		ImmediateQuad& quad = m_ImmediateQuads.emplace_back();

		const glm::vec2 half = size * 0.5f;
		const float c = cos(rotation);
		const float s = sin(rotation);

		// Same corner order as QuadDrawable: bottom-left, bottom-right, top-right, top-left.
		const glm::vec2 corners[4] = { { -half.x, -half.y }, { half.x, -half.y }, { half.x, half.y }, { -half.x, half.y } };

		for (int i = 0; i < 4; i++)
		{
			const glm::vec2 rotated = { corners[i].x * c - corners[i].y * s, corners[i].x * s + corners[i].y * c };
			quad.Vertices[i] = glm::vec3(rotated + position, 0.0f);
		}

		quad.Bounds = AABB2D::FromPoints(quad.Vertices);
		quad.Colour = colour;
		quad.TextureRef = texture;
		quad.Layer = layer;
	}

	void BatchRenderer2D::Render(const glm::mat4& viewProjection, std::vector<std::weak_ptr<Drawable>>& drawables)
	{
		NE_SCOPE_TIMER("BatchRenderer2D", "Render");

		m_Stats = RenderStats();
		m_FrameQuadCount = 0;

		{
			NE_SCOPE_TIMER("BatchRenderer2D", "Gather + Cull");
			GatherItems(CalculateViewBounds(viewProjection), drawables);
		}
		{
			NE_SCOPE_TIMER("BatchRenderer2D", "Sort");
			SortItems();
		}
		{
			NE_SCOPE_TIMER("BatchRenderer2D", "Batch + Draw");
			DrawItems(viewProjection);
		}

		// Immediate-mode quads only live for one frame.
		m_ImmediateQuads.clear();
	}

	void BatchRenderer2D::GatherItems(const AABB2D& view, std::vector<std::weak_ptr<Drawable>>& drawables)
	{
		m_Items.clear();
		bool foundExpired = false;

		for (const std::weak_ptr<Drawable>& weak : drawables)
		{
			// lock() gives us a shared_ptr if the drawable is still alive, or nullptr if the
			// game has destroyed it. That's how drawables "unregister" themselves for free.
			std::shared_ptr<Drawable> drawablePtr = weak.lock();

			if (drawablePtr == nullptr)
			{
				foundExpired = true;
				continue;
			}

			const Drawable& drawable = *drawablePtr;

			if (drawable.IsVisible() == false)
			{
				continue;
			}

			m_Stats.Submitted++;

			// ---- Culling ----
			// Cheap box-vs-box test. Anything off screen never reaches the GPU.
			if (m_CullingEnabled && view.Overlaps(drawable.GetBounds()) == false)
			{
				m_Stats.Culled++;
				continue;
			}

			// Store pointers into the drawable's own cached data. That's safe because the game
			// still owns the drawable, and no game code runs until we've finished drawing.
			const Material& material = drawable.GetMaterial();
			m_Items.push_back({ drawable.GetLayer(), drawable.GetWorldVertices().data(), drawable.GetShape().TexCoords.data(), material.GetColour(), material.GetBaseTexture() });
		}

		// Remove dead entries (keeps the order of the rest, so draw order stays stable).
		if (foundExpired)
		{
			std::erase_if(drawables, [](const std::weak_ptr<Drawable>& weak) { return weak.expired(); });
		}

		// Immediate quads are added after drawables, so within one layer they draw on top.
		for (const ImmediateQuad& quad : m_ImmediateQuads)
		{
			m_Stats.Submitted++;

			if (m_CullingEnabled && view.Overlaps(quad.Bounds) == false)
			{
				m_Stats.Culled++;
				continue;
			}

			m_Items.push_back({ quad.Layer, quad.Vertices.data(), s_QuadTexCoords.data(), quad.Colour, quad.TextureRef });
		}
	}

	void BatchRenderer2D::SortItems()
	{
		// Sort by layer only. Sorting by texture too would mean fewer batches, but would break
		// draw order for overlapping transparent shapes, so we rely on the 32 texture slots instead.
		auto byLayer = [](const RenderItem& a, const RenderItem& b) { return a.Layer < b.Layer; };

		// Most frames nothing changes layer, so checking first (O(n)) usually skips the sort (O(n log n)).
		if (std::is_sorted(m_Items.begin(), m_Items.end(), byLayer) == false)
		{
			// stable_sort keeps the original order of items on the same layer.
			std::stable_sort(m_Items.begin(), m_Items.end(), byLayer);
		}
	}

	void BatchRenderer2D::DrawItems(const glm::mat4& viewProjection)
	{
		if (m_Items.empty())
		{
			return;
		}

		// State that's the same for every batch this frame: bind it once.
		m_CameraBuffer->SetData(&viewProjection, sizeof(glm::mat4));
		m_Pipeline->Bind();			// must be first: the others are bound "into" the pipeline layout
		m_CameraBuffer->Bind();
		m_IndexBuffer->Bind();

		m_QuadCount = 0;
		m_TextureSlotCount = 1;

		for (const RenderItem& item : m_Items)
		{
			if (m_FrameQuadCount >= RendererConfig::MaxQuadsPerFrame)
			{
				if (m_WarnedFrameLimit == false)
				{
					NE_ENGINE_WARN("BatchRenderer2D hit MaxQuadsPerFrame ({}). Extra quads are skipped. Raise it in RendererConfig.h", RendererConfig::MaxQuadsPerFrame);
					m_WarnedFrameLimit = true;
				}

				break;
			}

			// Batch full? Send it and start a new one.
			if (m_QuadCount >= RendererConfig::MaxQuadsPerBatch)
			{
				Flush();
			}

			// May also Flush() if all texture slots are taken.
			const int32_t textureSlot = GetTextureSlot(item.TextureRef);

			// Write the 4 vertices straight into the batch array.
			BatchVertex* vertex = &m_Vertices[m_QuadCount * 4];

			for (int i = 0; i < 4; i++)
			{
				vertex[i].Position = item.Vertices[i];
				vertex[i].Colour = item.Colour;
				vertex[i].TexCoord = item.TexCoords[i];
				vertex[i].TexIndex = textureSlot;
			}

			m_QuadCount++;
			m_FrameQuadCount++;
		}

		// Whatever is left over.
		Flush();
	}

	int32_t BatchRenderer2D::GetTextureSlot(Texture* texture)
	{
		if (texture == nullptr)
		{
			return 0;
		}

		// Already used in this batch? (Linear search is fine, there are at most 32 slots.)
		for (uint32_t i = 1; i < m_TextureSlotCount; i++)
		{
			if (m_TextureSlots[i] == texture)
			{
				return (int32_t)i;
			}
		}

		// Out of slots: draw what we have, then this texture starts the next batch.
		if (m_TextureSlotCount >= RendererConfig::MaxTextureSlots)
		{
			Flush();
		}

		m_TextureSlots[m_TextureSlotCount] = texture;
		return (int32_t)m_TextureSlotCount++;
	}

	void BatchRenderer2D::Flush()
	{
		if (m_QuadCount == 0)
		{
			return;
		}

		// 1. Upload this batch's vertices.
		m_VertexBuffer->SetData(m_Vertices.data(), m_QuadCount * 4 * sizeof(BatchVertex));
		m_VertexBuffer->Bind();

		// 2. Bind every texture this batch uses to its slot.
		for (uint32_t i = 0; i < m_TextureSlotCount; i++)
		{
			m_TextureSlots[i]->SetTextureIndex((int)i);
			m_TextureSlots[i]->Bind();
		}

		// 3. One draw call for the whole batch.
		m_Backend.DrawIndexed(m_QuadCount * 6);

		m_Stats.DrawCalls++;
		m_Stats.QuadsDrawn += m_QuadCount;

		// Reset for the next batch (slot 0 stays as white).
		m_QuadCount = 0;
		m_TextureSlotCount = 1;
	}

	AABB2D BatchRenderer2D::CalculateViewBounds(const glm::mat4& viewProjection)
	{
		// The view-projection matrix maps world space -> screen space (-1..1 on X and Y).
		// So its inverse maps the 4 screen corners back to world space, giving the area the
		// camera can see. (Exact for 2D orthographic cameras, which is all we have.)
		const glm::mat4 inverse = glm::inverse(viewProjection);

		const glm::vec4 screenCorners[4] =
		{
			{ -1.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, -1.0f, 0.0f, 1.0f },
			{  1.0f,  1.0f, 0.0f, 1.0f }, { -1.0f, 1.0f, 0.0f, 1.0f }
		};

		std::array<glm::vec2, 4> worldCorners;

		for (int i = 0; i < 4; i++)
		{
			const glm::vec4 world = inverse * screenCorners[i];
			worldCorners[i] = glm::vec2(world) / world.w;
		}

		return AABB2D::FromPoints(worldCorners);
	}
}
