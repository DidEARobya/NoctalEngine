#pragma once
#include "NoctalEngine/Core.h"
#include "NoctalEngine/Input/LayerStack.h"

namespace NoctalEngine
{
	class Window;
	class Event;
	class WindowClosedEvent;
	class Layer;

	class NOCTAL_ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		bool CloseApplication(const WindowClosedEvent& closeEvent);

	private:
		Window* m_Window = nullptr;
		bool m_AppRunning = true;

		LayerStack m_LayerStack;
	};

	Application* CreateApplication();
}