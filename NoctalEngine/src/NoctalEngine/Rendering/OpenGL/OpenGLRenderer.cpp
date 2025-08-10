#include "OpenGLRenderer.h"
#include "GLAD/include/glad/glad.h"
#include <SDL3/SDL.h>

void OpenGLRenderer::Init(const NoctalEngine::Window* windowRef)
{
	int status = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
	NE_ENGINE_ASSERT(stats, "GLAD failed to initialise (WindowAPI)");
	NE_ENGINE_INFO("GLAD successfully loaded");
}

void OpenGLRenderer::Destroy()
{
}

void OpenGLRenderer::BeginRender()
{
}

void OpenGLRenderer::Render()
{
}

void OpenGLRenderer::EndRender()
{
}

void OpenGLRenderer::OnWindowResize(const uint32_t width, const uint32_t height)
{
}
