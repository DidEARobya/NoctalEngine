#include "nepch.h"
#include "Renderer.h"
#include "NoctalEngine/Application/Application.h"
#include "NoctalEngine/Rendering/Drawables/Shapes.h"
#include "NoctalEngine/Rendering/Textures/Texture.h"
#include "NoctalEngine/Rendering/Shaders/Shader.h"

// The only place that knows which API backend exists.
// To add another API: include its renderer here and create it in Init().
#include "D3D12/D3D12Renderer.h"

namespace NoctalEngine
{
	std::unique_ptr<Renderer> Renderer::s_Instance = std::make_unique<Renderer>();

	void Renderer::Init(const Window* windowRef)
	{
		if (m_WrappedRenderer == nullptr)
		{
			m_WrappedRenderer = std::make_unique<D3D12Renderer>();
		}

		NE_ENGINE_ASSERT(m_WrappedRenderer, "Renderer failed to create WrappedRenderer");
		m_WrappedRenderer->Init(windowRef);

		// The batch renderer creates its buffers/pipeline through the backend, so it has to
		// come after the backend is up.
		m_BatchRenderer = std::make_unique<BatchRenderer2D>(*m_WrappedRenderer);
		m_BatchRenderer->Init();
	}

	void Renderer::Destroy()
	{
		NE_ENGINE_ASSERT(m_WrappedRenderer, "WrappedRenderer doesn't exist");

		// Release our GPU resources before the backend tears the device down.
		m_BatchRenderer->Shutdown();
		m_BatchRenderer.reset();
		m_TextureCache.clear();

		m_WrappedRenderer->Destroy();
	}

	void Renderer::BeginRender()
	{
		NE_ENGINE_ASSERT(m_WrappedRenderer, "WrappedRenderer doesn't exist");
		m_WrappedRenderer->BeginFrame();
	}

	void Renderer::Render()
	{
		NE_ENGINE_ASSERT(m_WrappedRenderer, "WrappedRenderer doesn't exist");

		// Same camera source the old renderer used. Fetched here (after all layers have
		// updated) so the camera's latest movement is used for both culling and drawing.
		const glm::mat4& viewProjection = Application::Get().GetCameraViewProjection();
		m_BatchRenderer->Render(viewProjection, m_Drawables);

		DrawStatsWindow();
	}

	void Renderer::EndRender()
	{
		NE_ENGINE_ASSERT(m_WrappedRenderer, "WrappedRenderer doesn't exist");
		m_WrappedRenderer->EndFrame();
	}

	void Renderer::OnWindowResize(const uint32_t width, const uint32_t height)
	{
		NE_ENGINE_ASSERT(m_WrappedRenderer, "WrappedRenderer doesn't exist");
		m_WrappedRenderer->OnWindowResize(width, height);
	}

	const char* Renderer::GetVendor()
	{
		NE_ENGINE_ASSERT(m_WrappedRenderer, "WrappedRenderer doesn't exist");
		return m_WrappedRenderer->GetVendor();
	}

	const char* Renderer::GetRenderer()
	{
		NE_ENGINE_ASSERT(m_WrappedRenderer, "WrappedRenderer doesn't exist");
		return m_WrappedRenderer->GetRenderer();
	}

	const char* Renderer::GetVersion()
	{
		NE_ENGINE_ASSERT(m_WrappedRenderer, "WrappedRenderer doesn't exist");
		return m_WrappedRenderer->GetVersion();
	}

	std::shared_ptr<Shader> Renderer::LoadShader(const std::string& filePath)
	{
		NE_ENGINE_ASSERT(m_WrappedRenderer, "WrappedRenderer doesn't exist");
		return m_WrappedRenderer->LoadShader(filePath);
	}

	std::shared_ptr<Shader> Renderer::LoadShader(const std::string& customName, const std::string& filePath)
	{
		NE_ENGINE_ASSERT(m_WrappedRenderer, "WrappedRenderer doesn't exist");
		return m_WrappedRenderer->LoadShader(customName, filePath);
	}

	std::shared_ptr<Shader> Renderer::GetShader(const std::string& shaderName)
	{
		NE_ENGINE_ASSERT(m_WrappedRenderer, "WrappedRenderer doesn't exist");
		return m_WrappedRenderer->GetShader(shaderName);
	}

	std::shared_ptr<Shader> Renderer::CreateShader(const std::string& filePath)
	{
		NE_ENGINE_ASSERT(m_WrappedRenderer, "WrappedRenderer doesn't exist");
		return m_WrappedRenderer->CreateShader(filePath);
	}

	std::shared_ptr<Texture> Renderer::CreateTexture(const std::string& path, const std::string& assetDir)
	{
		NE_ENGINE_ASSERT(m_WrappedRenderer, "WrappedRenderer doesn't exist");

		std::string fullPath = assetDir;
		fullPath.append(path);

		// Texture cache: if this file is already loaded (and still in use), share it.
		// Fewer unique textures also means fewer texture slots used per batch.
		auto it = m_TextureCache.find(fullPath);

		if (it != m_TextureCache.end())
		{
			if (std::shared_ptr<Texture> cached = it->second.lock())
			{
				return cached;
			}
		}

		std::shared_ptr<Texture> texture = m_WrappedRenderer->CreateTexture(fullPath);
		m_TextureCache[fullPath] = texture;
		return texture;
	}

	std::shared_ptr<Drawable> Renderer::CreateDrawable(Geometry type, glm::vec2 position, glm::vec2 scale)
	{
		std::shared_ptr<Drawable> drawable;

		switch (type)
		{
			case Geometry::QUAD:
				drawable = std::make_shared<QuadDrawable>(position, scale);
				break;

			case Geometry::TRIANGLE:
				drawable = std::make_shared<TriangleDrawable>(position, scale);
				break;

			default:
				NE_ENGINE_ERROR("Geometry type is invalid");
				return nullptr;
		}

		m_Drawables.push_back(drawable);
		return drawable;
	}

	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& colour, int32_t layer, float rotation)
	{
		m_BatchRenderer->SubmitQuad(position, size, rotation, colour, nullptr, layer);
	}

	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture, const glm::vec4& tint, int32_t layer, float rotation)
	{
		m_BatchRenderer->SubmitQuad(position, size, rotation, tint, texture.get(), layer);
	}

	const RenderStats& Renderer::GetStats() const
	{
		return m_BatchRenderer->GetStats();
	}

	void Renderer::SetCullingEnabled(bool enabled)
	{
		m_BatchRenderer->SetCullingEnabled(enabled);
	}

	bool Renderer::IsCullingEnabled() const
	{
		return m_BatchRenderer->IsCullingEnabled();
	}

	void Renderer::DrawStatsWindow()
	{
		// Drawn from here (rather than ImGuiLayer) so the debugging code didn't need changing.
		if (ImGui::Begin("Renderer Stats"))
		{
			const RenderStats& stats = m_BatchRenderer->GetStats();

			ImGui::Text("Draw calls:   %u", stats.DrawCalls);
			ImGui::Text("Quads drawn:  %u", stats.QuadsDrawn);
			ImGui::Text("Submitted:    %u", stats.Submitted);
			ImGui::Text("Culled:       %u", stats.Culled);
			ImGui::Text("Registered drawables: %u", (uint32_t)m_Drawables.size());

			bool culling = m_BatchRenderer->IsCullingEnabled();

			if (ImGui::Checkbox("Frustum culling", &culling))
			{
				m_BatchRenderer->SetCullingEnabled(culling);
			}
		}

		ImGui::End();
	}
}
