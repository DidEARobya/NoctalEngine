#include "nepch.h"
#include "WindowsWindow.h"
#include "NoctalEngine/Input/InputManager.h"
#include <SDL3/SDL.h>

namespace NoctalEngine
{
	static bool s_IsSDLInitialised;

	Window* Window::Create(const WindowProperties& properties)
	{
		return new WindowsWindow(properties);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& properties)
	{
		Init(properties);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		InputManager::Update(this);
		SDL_GL_SwapWindow(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{

	}

	bool WindowsWindow::IsVSync() const
	{
		return false;
	}

	void WindowsWindow::Init(const WindowProperties& properties)
	{
		WindowData.Title = properties.Title;
		WindowData.Width = properties.Width;
		WindowData.Height = properties.Height;

		NE_ENGINE_INFO("Creating Window {0} ({1}, {2}) (WindowsWindow)", properties.Title, properties.Width, properties.Height);
		m_Window = SDL_CreateWindow(properties.Title.c_str(), properties.Width, properties.Height, 0);
	}

	void WindowsWindow::Shutdown()
	{
		SDL_DestroyWindow(m_Window);
		m_Window = nullptr;
	}
}