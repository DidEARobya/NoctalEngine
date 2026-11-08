#include "nepch.h"
#include "D3D12Window.h"
#include "NoctalEngine/Input/InputManager.h"

namespace NoctalEngine
{
	D3D12Window::D3D12Window(const WindowProperties& properties)
	{
		Init(properties);
	}

	D3D12Window::~D3D12Window()
	{
		Shutdown();
	}

	void D3D12Window::OnUpdate()
	{
		InputManager::Update(this);
	}

	void D3D12Window::Init(const WindowProperties& properties)
	{
		WindowData.Title = properties.Title;
		WindowData.Width = properties.Width;
		WindowData.Height = properties.Height;

		NE_ENGINE_INFO("Creating Window {0} ({1}, {2})", properties.Title, properties.Width, properties.Height);

		m_Window = SDL_CreateWindow(properties.Title.c_str(), properties.Width, properties.Height, SDL_WINDOW_RESIZABLE);
		NE_ENGINE_SDL_ASSERT(m_Window);
		NE_ENGINE_INFO("SDL successfully created Window");
	}

	void D3D12Window::Shutdown()
	{
		SDL_DestroyWindow(m_Window);
		m_Window = nullptr;
	}
}
