#pragma once
#include "NoctalEngine/Core.h"
#include "NoctalEngine/Input/LayerStack.h"
#include "NoctalEngine/Utility/Timer.h"

namespace NoctalEngine
{
	class Window;
	class Event;
	class WindowClosedEvent;
	class Layer;
	class ImGuiLayer;

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

		inline static Application& Get() { return *s_Instance; };
		inline Window& GetWindow() { return *m_Window; };

	private:
		Window* m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_AppRunning = true;
		Timer m_Timer;
		LayerStack m_LayerStack;
		inline static Application* s_Instance;
	};

	Application* CreateApplication();
}