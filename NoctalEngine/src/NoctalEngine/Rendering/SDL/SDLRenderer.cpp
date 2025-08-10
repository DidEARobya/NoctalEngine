#include "SDLRenderer.h"
#include "NoctalEngine/Window/Window.h"
#include <SDL3/SDL.h>

std::unique_ptr<WrappedRenderer> WrappedRenderer::m_Instance = std::make_unique<SDLRenderer>();

SDLRenderer::~SDLRenderer()
{
	Destroy();
}

void SDLRenderer::Init(const NoctalEngine::Window* windowRef)
{
	m_SDLRenderer = SDL_CreateRenderer(windowRef->GetSDLWindow(), nullptr);
}

void SDLRenderer::Destroy()
{
	SDL_DestroyRenderer(m_SDLRenderer);
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


