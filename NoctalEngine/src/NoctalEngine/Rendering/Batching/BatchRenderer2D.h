#pragma once
#include "NoctalEngine/Rendering/RendererConfig.h"
#include "NoctalEngine/Rendering/Geometry/Bounds.h"
#include <glm/glm.hpp>
#include <array>
#include <memory>
#include <vector>

class WrappedRenderer;
class Drawable;

namespace NoctalEngine
{
	class Texture;
	class VertexBuffer;
	class IndexBuffer;
	class ConstantBuffer;
	class Pipeline;

	// Numbers from the last frame, shown in the "Renderer Stats" ImGui window.
	struct RenderStats
	{
		uint32_t DrawCalls = 0;		// How many times we asked the GPU to draw.
		uint32_t QuadsDrawn = 0;	// Shapes that made it to the GPU.
		uint32_t Submitted = 0;		// Visible shapes that were considered this frame.
		uint32_t Culled = 0;		// Shapes skipped because they were off screen.
	};

	// ---------------------------------------------------------------------------------
	// BatchRenderer2D
	// The API-agnostic heart of the 2D renderer. Every frame it:
	//   1. GATHERS  every drawable + immediate-mode quad.
	//   2. CULLS    anything whose bounding box is outside the camera's view.
	//   3. SORTS    what's left by render layer (stable, so creation order is kept within a layer).
	//   4. BATCHES  them: writes each shape's 4 vertices into one big array and only issues a
	//               draw call when the array is full or we run out of texture slots.
	//
	// Without batching, 1600 shapes = 1600 draw calls (each with its own state changes).
	// With batching, 1600 shapes = usually 1 draw call. Draw calls are the expensive part.
	//
	// It only talks to the abstract interfaces (VertexBuffer, Texture, Pipeline, WrappedRenderer...),
	// so it works unchanged with any graphics API.
	// ---------------------------------------------------------------------------------
	class BatchRenderer2D
	{
	public:
		explicit BatchRenderer2D(WrappedRenderer& backend);
		~BatchRenderer2D();

		// Creates the GPU resources (needs the backend to be initialised first).
		void Init();
		void Shutdown();

		// Immediate mode: queue a quad for this frame only (great for particles, debug shapes, etc.).
		// 'texture' may be nullptr. It must stay alive until the end of the frame.
		void SubmitQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& colour, Texture* texture, int32_t layer);

		// Draws everything. Expired drawables (the owner let go of them) are removed from the list.
		void Render(const glm::mat4& viewProjection, std::vector<std::weak_ptr<Drawable>>& drawables);

		const RenderStats& GetStats() const { return m_Stats; }

		void SetCullingEnabled(bool enabled) { m_CullingEnabled = enabled; }
		bool IsCullingEnabled() const { return m_CullingEnabled; }

	private:
		// Exactly what the vertex shader receives (see Batch2DVS.hlsl). 40 bytes.
		struct BatchVertex
		{
			glm::vec3 Position;		// Already in world space (transformed on the CPU).
			glm::vec4 Colour;
			glm::vec2 TexCoord;
			int32_t TexIndex;		// Which texture slot of this batch to sample.
		};

		// One shape ready to be batched. Points into data owned elsewhere, to avoid copying.
		struct RenderItem
		{
			int32_t Layer;
			const glm::vec3* Vertices;	// 4 world-space corners
			const glm::vec2* TexCoords;	// 4 UVs
			glm::vec4 Colour;
			Texture* TextureRef;		// nullptr = white
		};

		// Storage for immediate-mode quads (the drawables store their own vertices).
		struct ImmediateQuad
		{
			std::array<glm::vec3, 4> Vertices;
			AABB2D Bounds;
			glm::vec4 Colour;
			Texture* TextureRef;
			int32_t Layer;
		};

	private:
		void GatherItems(const AABB2D& view, std::vector<std::weak_ptr<Drawable>>& drawables);
		void SortItems();
		void DrawItems(const glm::mat4& viewProjection);

		// Returns the slot this texture uses in the current batch (adding it if needed).
		int32_t GetTextureSlot(Texture* texture);
		void Flush();

		static AABB2D CalculateViewBounds(const glm::mat4& viewProjection);

	private:
		WrappedRenderer& m_Backend;

		// GPU resources (created through the backend, so they're API-specific underneath).
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<ConstantBuffer> m_CameraBuffer;
		std::unique_ptr<Pipeline> m_Pipeline;
		std::shared_ptr<Texture> m_WhiteTexture;

		// CPU side of the current batch.
		std::vector<BatchVertex> m_Vertices;
		uint32_t m_QuadCount = 0;
		std::array<Texture*, RendererConfig::MaxTextureSlots> m_TextureSlots{};
		uint32_t m_TextureSlotCount = 1;	// Slot 0 is always the white texture.

		// Per-frame lists. Kept as members so their memory is reused every frame (no reallocations).
		std::vector<RenderItem> m_Items;
		std::vector<ImmediateQuad> m_ImmediateQuads;
		uint32_t m_FrameQuadCount = 0;

		bool m_CullingEnabled = true;
		bool m_WarnedFrameLimit = false;
		RenderStats m_Stats;
	};
}
