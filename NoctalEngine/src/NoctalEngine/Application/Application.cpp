#include "nepch.h"
#include "Application.h"
#include "NoctalEngine/Window/Window.h"
#include "NoctalEngine/Events/Event.h"

namespace NoctalEngine
{
#define BIND_EVENT(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT(OnEvent));
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

	void OnEvent(Event& event)
	{
		NE_ENGINE_INFO("Event");
	}
}