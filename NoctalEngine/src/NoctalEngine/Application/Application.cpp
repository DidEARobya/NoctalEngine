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
	Application::Application()
	{
		m_Window = WindowManager::CreateWindowInternal();
		m_Window->SetEventCallback(NOCTAL_BIND_EVENT_FN(OnEvent));

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
	}

	bool Application::CloseApplication(const WindowClosedEvent& closeEvent)
	{
		m_AppRunning = false;
		return true;
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowClosedEvent>(NOCTAL_BIND_EVENT_FN(CloseApplication));

		NE_ENGINE_TRACE("Event");
		InputManager::OnEvent(event);
	}
}