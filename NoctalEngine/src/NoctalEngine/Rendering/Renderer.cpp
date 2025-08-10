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

		NE_ENGINE_ASSERT(m_Instance->m_WrappedRenderer, "Renderer failed to create WrappedRenderer (Renderer)");
		m_Instance->m_WrappedRenderer->Init(windowRef);
	}

	void Renderer::Destroy()
	{
		if (m_Instance->m_WrappedRenderer == nullptr)
		{
			NE_ENGINE_FATAL("WrappedRenderer is uninitalised (Renderer)");
			return;
		}

		m_Instance->m_WrappedRenderer->Destroy();
	}

	void Renderer::BeginRender()
	{
		if (m_Instance->m_WrappedRenderer == nullptr)
		{
			NE_ENGINE_FATAL("WrappedRenderer is uninitalised (Renderer)");
			return;
		}

		m_Instance->m_WrappedRenderer->BeginRender();
	}

	void Renderer::Render()
	{
		if (m_Instance->m_WrappedRenderer == nullptr)
		{
			NE_ENGINE_FATAL("WrappedRenderer is uninitalised (Renderer)");
			return;
		}

		m_Instance->m_WrappedRenderer->Render();
	}

	void Renderer::EndRender()
	{
		if (m_Instance->m_WrappedRenderer == nullptr)
		{
			NE_ENGINE_FATAL("WrappedRenderer is uninitalised (Renderer)");
			return;
		}

		m_Instance->m_WrappedRenderer->EndRender();
	}

	void Renderer::OnWindowResize(const uint32_t width, const uint32_t height)
	{
		if (m_Instance->m_WrappedRenderer == nullptr)
		{
			NE_ENGINE_FATAL("WrappedRenderer is uninitalised (Renderer)");
			return;
		}

		m_Instance->m_WrappedRenderer->OnWindowResize(width, height);
	}
}