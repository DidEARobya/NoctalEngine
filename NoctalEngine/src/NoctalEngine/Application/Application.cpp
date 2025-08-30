#include "nepch.h"
#include "Application.h"
#include "NoctalEngine/Window/WindowManager.h"
#include "NoctalEngine/Window/Window.h"
#include "NoctalEngine/Events/Event.h"
#include "NoctalEngine/Events/WindowEvents.h"
#include "NoctalEngine/Rendering/Renderer.h"
#include "NoctalEngine/Input/InputManager.h"
#include "NoctalEngine/Input/Layer.h"
#include "NoctalEngine/Debugging/ImGuiLayer.h"
#include "AppLayer.h"
#include <conio.h>

namespace NoctalEngine
{
	Application::Application() : m_Minimized(false)
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

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
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
			m_DeltaTime = m_Timer.Mark();
			m_Window->OnUpdate();

			if (m_Minimized == false)
			{
				{
					NOCTAL_SCOPE_TIMER("Renderer::BeginRender");
					Renderer::Instance().BeginRender();
				}

				for (Layer* layer : m_LayerStack)
				{
					layer->OnUpdate(m_DeltaTime);
				}

				m_GameLayer->LateUpdate(m_DeltaTime);

				{
					NOCTAL_SCOPE_TIMER("Renderer::Render");
					Renderer::Instance().Render();
				}

				{
					NOCTAL_SCOPE_TIMER("Renderer::EndRender");
					Renderer::Instance().EndRender();
				}
			}

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
	bool Application::ResizeApplication(const WindowResizeEvent& resizeEvent)
	{
		if (resizeEvent.GetWidth() == 0 || resizeEvent.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		NoctalEngine::Renderer::Instance().OnWindowResize(resizeEvent.GetWidth(), resizeEvent.GetHeight());
		return true;
	}
	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowClosedEvent>(NOCTAL_BIND_EVENT_FN(CloseApplication));
		dispatcher.Dispatch<WindowResizeEvent>(NOCTAL_BIND_EVENT_FN(ResizeApplication));


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