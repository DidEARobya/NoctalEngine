#include "nepch.h"
#include "WindowManager.h"
#include "GLAD/include/glad/glad.h"
#include <SDL3/SDL.h>

namespace NoctalEngine
{
	bool WindowManager::m_Initialised = false;

	void WindowManager::InitWindowAPI()
	{
		bool sdlSuccess = SDL_Init(SDL_INIT_VIDEO);
		NE_ENGINE_ASSERT(sdlSuccess, "SDL failed to initialise");
		m_Initialised = sdlSuccess;

		NE_ENGINE_INFO("SDL Initialised");
	}

	void WindowManager::TerminateWindowAPI()
	{
		SDL_Quit();
		NE_ENGINE_INFO("SDL Quit");
	}

	Window* WindowManager::CreateWindowInternal(const WindowProperties& properties)
	{
		NE_ENGINE_ASSERT(m_Initialised, "WindowManager is not initialised when trying to create new Window");

		Window* window = Window::Create(properties);
		NE_ENGINE_ASSERT(window, "WindowManager failed to create new Window");

		NE_ENGINE_INFO("WindowManager created new Window");
		return window;
	}

	void WindowManager::DestroyWindow(const Window* window)
	{
		NE_ENGINE_INFO("WindowManager destroyed Window");
		delete window;
		window = nullptr;
	}
}

