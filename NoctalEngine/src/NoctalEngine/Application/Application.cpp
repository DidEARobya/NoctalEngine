#include "nepch.h"
#include "Application.h"
#include "NoctalEngine/Window/WindowManager.h"
#include "NoctalEngine/Window/Window.h"
#include "NoctalEngine/Events/Event.h"
#include "NoctalEngine/Events/WindowEvents.h"
#include "NoctalEngine/Rendering/Renderer.h"
#include "NoctalEngine/Input/InputManager.h"
#include <conio.h>

namespace NoctalEngine
{
#define BIND_EVENT(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = WindowManager::CreateWindowInternal();
		m_Window->SetEventCallback(BIND_EVENT(OnEvent));

		Renderer::Instance().Init(m_Window);
	}

	Application::~Application()
	{
		Renderer::Instance().Destroy();
		WindowManager::DestroyWindow(m_Window);
		WindowManager::TerminateWindowAPI();
	}

	void Application::Run()
	{
		while (m_AppRunning)
		{
			m_Window->OnUpdate();

			Renderer::Instance().BeginRender();
			Renderer::Instance().Render();
			Renderer::Instance().EndRender();

			//OnUpdateEvent event;
			//OnEvent(event);
		}

		_getch();
	}

	void Application::OnEvent(Event& event)
	{
		NE_ENGINE_INFO("Event");
		InputManager::OnEvent(event);
	}
}