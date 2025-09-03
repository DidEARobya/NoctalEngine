#include "nepch.h"
#include "Renderer.h"
#include "OpenGL/OpenGLRenderer.h"

namespace NoctalEngine
{
	std::unique_ptr<Renderer> Renderer::s_Instance = std::make_unique<Renderer>();

	void Renderer::Init(const Window* windowRef)
	{
		if (s_Instance->m_WrappedRenderer == nullptr)
		{
			s_Instance->m_WrappedRenderer = std::make_unique<OpenGLRenderer>();
		}

		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "Renderer failed to create WrappedRenderer");
		s_Instance->m_WrappedRenderer->Init(windowRef);
	}

	void Renderer::Destroy()
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");
		s_Instance->m_WrappedRenderer->Destroy();
	}

	void Renderer::BeginRender()
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");
		s_Instance->m_WrappedRenderer->BeginRender();
	}

	void Renderer::Render()
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");
		s_Instance->m_WrappedRenderer->Render();
	}

	void Renderer::EndRender()
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");
		s_Instance->m_WrappedRenderer->EndRender();
	}

	void Renderer::OnWindowResize(const uint32_t width, const uint32_t height)
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");
		s_Instance->m_WrappedRenderer->OnWindowResize(width, height);
	}

	const char* Renderer::GetVendor()
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");

		return s_Instance->m_WrappedRenderer->GetVendor();
	}

	const char* Renderer::GetRenderer()
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");
		return s_Instance->m_WrappedRenderer->GetRenderer();
	}

	const char* Renderer::GetVersion()
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");
		return s_Instance->m_WrappedRenderer->GetVersion();
	}
	std::shared_ptr<Shader> Renderer::LoadShader(const std::string& filePath)
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");
		return s_Instance->m_WrappedRenderer->LoadShader(filePath);
	}

	std::shared_ptr<Shader> Renderer::LoadShader(const std::string& customName, const std::string& filePath)
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");
		return s_Instance->m_WrappedRenderer->LoadShader(customName, filePath);
	}
	
	std::shared_ptr<Shader> Renderer::GetShader(const std::string& shaderName)
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");
		return s_Instance->m_WrappedRenderer->GetShader(shaderName);
	}	
	std::shared_ptr<Shader> Renderer::CreateShader(const std::string& filePath)
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");
		return s_Instance->m_WrappedRenderer->CreateShader(filePath);
	}
	std::shared_ptr<Texture> Renderer::CreateTexture(const std::string& path, const std::string& assetDir)
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");

		std::string dir = assetDir;
		dir.append(path);
		return s_Instance->m_WrappedRenderer->CreateTexture(dir);
	}

	std::shared_ptr<Drawable> Renderer::CreateDrawable(Geometry type, glm::vec2 position, glm::vec2 scale)
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");
		return s_Instance->m_WrappedRenderer->CreateDrawable(type, position, scale);
	}

	void Renderer::SetIndexBuffer(IndexBuffer* indexBuffer)
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");
		return s_Instance->m_WrappedRenderer->SetIndexBuffer(indexBuffer);
	}

	void Renderer::DrawIndexed()
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");
		return s_Instance->m_WrappedRenderer->DrawIndexed();
	}
	void Renderer::DrawIndexed(uint32_t indices, uint32_t indexCount)
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");
		return s_Instance->m_WrappedRenderer->DrawIndexed(indices, indexCount);
	}
}