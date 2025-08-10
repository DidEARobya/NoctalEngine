#include "SDLRenderer.h"
#include "NoctalEngine/Window/Window.h"
#include <SDL3/SDL.h>

void SDLRenderer::Init(const NoctalEngine::Window* windowRef)
{
	m_SDLRenderer = SDL_CreateRenderer(windowRef->GetSDLWindow(), nullptr);
}

void SDLRenderer::Destroy()
{
	if (m_SDLRenderer != nullptr)
	{
		SDL_DestroyRenderer(m_SDLRenderer);
		m_SDLRenderer = nullptr;
	}
}

void SDLRenderer::BeginRender()
{
}

void SDLRenderer::Render()
{
}

void SDLRenderer::EndRender()
{
}

void SDLRenderer::OnWindowResize(const uint32_t width, const uint32_t height)
{
}


