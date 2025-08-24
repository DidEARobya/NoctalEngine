#include "Renderer.h"
#include "SDL/SDLRenderer.h"
#include "OpenGL/OpenGLRenderer.h"

namespace NoctalEngine
{
	std::unique_ptr<Renderer> Renderer::m_Instance = std::make_unique<Renderer>();

	void Renderer::Init(const Window* windowRef)
	{
		if (m_Instance->m_WrappedRenderer == nullptr)
		{
			m_Instance->m_WrappedRenderer = std::make_unique<OpenGLRenderer>();
		}

		NE_ENGINE_ASSERT(m_Instance->m_WrappedRenderer, "Renderer failed to create WrappedRenderer");
		m_Instance->m_WrappedRenderer->Init(windowRef);
	}

	void Renderer::Destroy()
	{
		if (m_Instance->m_WrappedRenderer == nullptr)
		{
			NE_ENGINE_FATAL("WrappedRenderer is uninitalised");
			return;
		}

		m_Instance->m_WrappedRenderer->Destroy();
	}

	void Renderer::BeginRender()
	{
		if (m_Instance->m_WrappedRenderer == nullptr)
		{
			NE_ENGINE_FATAL("WrappedRenderer is uninitalised");
			return;
		}

		m_Instance->m_WrappedRenderer->BeginRender();
	}

	void Renderer::Render()
	{
		if (m_Instance->m_WrappedRenderer == nullptr)
		{
			NE_ENGINE_FATAL("WrappedRenderer is uninitalised");
			return;
		}

		m_Instance->m_WrappedRenderer->Render();
	}

	void Renderer::EndRender()
	{
		if (m_Instance->m_WrappedRenderer == nullptr)
		{
			NE_ENGINE_FATAL("WrappedRenderer is uninitalised");
			return;
		}

		m_Instance->m_WrappedRenderer->EndRender();
	}

	void Renderer::OnWindowResize(const uint32_t width, const uint32_t height)
	{
		if (m_Instance->m_WrappedRenderer == nullptr)
		{
			NE_ENGINE_FATAL("WrappedRenderer is uninitalised");
			return;
		}

		m_Instance->m_WrappedRenderer->OnWindowResize(width, height);
	}

	const char* Renderer::GetVendor()
	{
		if (m_Instance->m_WrappedRenderer == nullptr)
		{
			NE_ENGINE_FATAL("WrappedRenderer is uninitalised");
			return "";
		}

		return m_Instance->m_WrappedRenderer->GetVendor();
	}

	const char* Renderer::GetRenderer()
	{
		if (m_Instance->m_WrappedRenderer == nullptr)
		{
			NE_ENGINE_FATAL("WrappedRenderer is uninitalised");
			return "";
		}

		return m_Instance->m_WrappedRenderer->GetRenderer();
	}

	const char* Renderer::GetVersion()
	{
		if (m_Instance->m_WrappedRenderer == nullptr)
		{
			NE_ENGINE_FATAL("WrappedRenderer is uninitalised");
			return "";
		}

		return m_Instance->m_WrappedRenderer->GetVersion();
	}	
	
	Shader* Renderer::CreateShader(const std::string& vertexSource, const std::string& pixelSource)
	{
		if (m_Instance->m_WrappedRenderer == nullptr)
		{
			NE_ENGINE_FATAL("WrappedRenderer is uninitalised");
			return nullptr;
		}

		return m_Instance->m_WrappedRenderer->CreateShader(vertexSource, pixelSource);
	}

	VertexBuffer* Renderer::CreateVertexBuffer(float* vertices, uint32_t size, const BufferLayout& layout)
	{
		if (m_Instance->m_WrappedRenderer == nullptr)
		{
			NE_ENGINE_FATAL("WrappedRenderer is uninitalised");
			return nullptr;
		}

		return m_Instance->m_WrappedRenderer->CreateVertexBuffer(vertices, size, layout);
	}
	IndexBuffer* Renderer::CreateIndexBuffer(uint32_t* indices, uint32_t size)
	{
		if (m_Instance->m_WrappedRenderer == nullptr)
		{
			NE_ENGINE_FATAL("WrappedRenderer is uninitalised");
			return nullptr;
		}

		return m_Instance->m_WrappedRenderer->CreateIndexBuffer(indices, size);
	}
}