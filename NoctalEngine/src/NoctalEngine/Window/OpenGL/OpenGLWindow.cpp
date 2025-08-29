#include "nepch.h"
#include "OpenGLWindow.h"
#include "NoctalEngine/Input/InputManager.h"

namespace NoctalEngine
{
	OpenGLWindow::OpenGLWindow(const WindowProperties& properties)
	{
		Init(properties);
	}

	OpenGLWindow::~OpenGLWindow()
	{
		Shutdown();
	}

	void OpenGLWindow::OnUpdate()
	{
		InputManager::Update(this);
	}

	void OpenGLWindow::SetVSync(bool enabled)
	{
		SDL_GL_SetSwapInterval(enabled);
	}

	bool OpenGLWindow::IsVSync() const
	{
		int returnVal;

		NE_ENGINE_SDL_ASSERT(SDL_GL_GetSwapInterval(&returnVal))
		return returnVal < 1;
	}

	void OpenGLWindow::Init(const WindowProperties& properties)
	{
		WindowData.Title = properties.Title;
		WindowData.Width = properties.Width;
		WindowData.Height = properties.Height;

		NE_ENGINE_INFO("Creating Window {0} ({1}, {2})", properties.Title, properties.Width, properties.Height);

		bool loadGLSuccess = SDL_GL_LoadLibrary(NULL);
		NE_ENGINE_ASSERT(loadGLSuccess, "SDL failed to load OpenGL Library");
		NE_ENGINE_INFO("SDL successfully loaded OpenGL");

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		m_Window = SDL_CreateWindow(properties.Title.c_str(), properties.Width, properties.Height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		NE_ENGINE_SDL_ASSERT(m_Window);
		NE_ENGINE_INFO("SDL successfully created Window");
	}

	void OpenGLWindow::Shutdown()
	{
		SDL_DestroyWindow(m_Window);
		m_Window = nullptr;
	}
}