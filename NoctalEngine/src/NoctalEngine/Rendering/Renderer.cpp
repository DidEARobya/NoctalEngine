#include "Renderer.h"
#include "WrappedRenderer.h"

namespace NoctalEngine
{
	std::unique_ptr<Renderer> Renderer::m_Instance;

	void Renderer::Init(const Window* windowRef)
	{
		WrappedRenderer::Instance().Init(windowRef);
	}

	void Renderer::Destroy()
	{
		WrappedRenderer::Instance().Destroy();
	}

	void Renderer::BeginRender()
	{
		WrappedRenderer::Instance().BeginRender();
	}

	void Renderer::Render()
	{
		WrappedRenderer::Instance().Render();
	}

	void Renderer::EndRender()
	{
		WrappedRenderer::Instance().EndRender();
	}

	void Renderer::OnWindowResize(const uint32_t width, const uint32_t height)
	{
		WrappedRenderer::Instance().OnWindowResize(width, height);
	}
}