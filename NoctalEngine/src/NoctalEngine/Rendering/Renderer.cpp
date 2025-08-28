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
	
	Shader* Renderer::CreateShader(const std::string& vertexSource, const std::string& pixelSource)
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");

		return s_Instance->m_WrappedRenderer->CreateShader(vertexSource, pixelSource);
	}

	VertexBuffer* Renderer::CreateVertexBuffer(float* vertices, uint32_t size, const BufferLayout& layout)
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");

		return s_Instance->m_WrappedRenderer->CreateVertexBuffer(vertices, size, layout);
	}
	IndexBuffer* Renderer::CreateIndexBuffer(uint32_t* indices, uint32_t size)
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");

		return s_Instance->m_WrappedRenderer->CreateIndexBuffer(indices, size);
	}

	std::shared_ptr<Texture> Renderer::CreateTexture(const std::string& path, const std::string& assetDir)
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");

		std::string dir = assetDir;
		dir.append(path);
		return s_Instance->m_WrappedRenderer->CreateTexture(dir);
	}

	std::shared_ptr<Drawable> Renderer::CreateDrawable(Geometry geometry)
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");

		return s_Instance->m_WrappedRenderer->CreateDrawable(geometry);
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

	const glm::mat4& Renderer::GetCameraViewProjectionMatrix() const
	{
		NE_ENGINE_ASSERT(s_Instance->m_WrappedRenderer, "WrappedRenderer doesn't exist");

		return s_Instance->m_WrappedRenderer->GetCameraViewProjectionMatrix();
	}
}