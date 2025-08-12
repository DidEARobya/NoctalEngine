#include "nepch.h"
#include "Application.h"
#include "NoctalEngine/Window/WindowManager.h"
#include "NoctalEngine/Window/Window.h"
#include "NoctalEngine/Events/Event.h"
#include "NoctalEngine/Events/WindowEvents.h"
#include "NoctalEngine/Rendering/Renderer.h"
#include "NoctalEngine/Input/InputManager.h"
#include "NoctalEngine/Input/Layer.h"
#include <conio.h>

namespace NoctalEngine
{
	Application::Application()
	{
		s_Instance = this;

		NE_ENGINE_ASSERT(s_Instance != nullptr, "Application already exists");

		WindowProperties properties = WindowProperties();
		properties.TestAPI = TestGraphicsAPI::OPEN_GL;

		WindowManager::InitWindowAPI();
		m_Window = WindowManager::CreateWindowInternal(properties);
		NE_ENGINE_ASSERT(m_Window, "Window creation failed");

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
			const auto deltaTime = m_Timer.Mark();
			m_Window->OnUpdate();

			Renderer::Instance().BeginRender();

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(deltaTime);
			}

			Renderer::Instance().Render();
			Renderer::Instance().EndRender();

			//OnUpdateEvent event;
			//OnEvent(event);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}


	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
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

		for (auto iterator = m_LayerStack.end(); iterator != m_LayerStack.begin();)
		{
			(*--iterator)->OnEvent(event);

			if (event.IsHandled() == true)
			{
				break;
			}
		}

		NE_ENGINE_TRACE("Event");
		InputManager::OnEvent(event);
	}
}