#include "OpenGLRenderer.h"

void OpenGLRenderer::Init(const NoctalEngine::Window* windowRef)
{
	m_GLContext = SDL_GL_CreateContext(windowRef->GetSDLWindow());
	NE_ENGINE_SDL_ASSERT(m_GLContext);
	NE_ENGINE_INFO("SDL successfully loaded OpenGL_Context");

	int status = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
	NE_ENGINE_ASSERT(status, "GLAD failed to initialise");
	NE_ENGINE_INFO("GLAD successfully loaded");

	if (SDL_GL_SetSwapInterval(1) == false)
	{
		NE_ENGINE_ERROR(SDL_GetError());
	}

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0, 0, windowRef->GetWidth(), windowRef->GetHeight());
}

void OpenGLRenderer::Destroy()
{
}

void OpenGLRenderer::BeginRender()
{
	glClearColor(1.0f, 0.5f, 1.0f, 0.0f);
}

void OpenGLRenderer::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderer::EndRender()
{
	glLoadIdentity();
}

void OpenGLRenderer::OnWindowResize(const uint32_t width, const uint32_t height)
{
}
