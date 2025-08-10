#include "nepch.h"
#include "Application.h"
#include "NoctalEngine/Window/Window.h"
#include "NoctalEngine/Events/Event.h"
#include "NoctalEngine/Rendering/Renderer.h"

namespace NoctalEngine
{
#define BIND_EVENT(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT(OnEvent));

		Renderer::Instance().Init(m_Window.get());
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_AppRunning)
		{
			m_Window->OnUpdate();

			Renderer::Instance().BeginRender();
			Renderer::Instance().Render();
			Renderer::Instance().EndRender();
		}
	}

	void Application::OnEvent(Event& event)
	{
		NE_ENGINE_INFO("Event");
	}
}