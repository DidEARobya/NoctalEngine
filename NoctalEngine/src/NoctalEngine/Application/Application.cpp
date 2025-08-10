#include "nepch.h"
#include "Application.h"
#include "NoctalEngine/Window/Window.h"

namespace NoctalEngine
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_AppRunning)
		{
			m_Window->OnUpdate();
		}
	}
}