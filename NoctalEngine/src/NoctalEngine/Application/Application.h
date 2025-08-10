#pragma once
#include "NoctalEngine/Core.h"

namespace NoctalEngine
{
	class Window;
	class Event;
	class WindowClosedEvent;

	class NOCTAL_ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& event);

		bool CloseApplication(const WindowClosedEvent& closeEvent);

	private:
		Window* m_Window = nullptr;
		bool m_AppRunning = true;
	};

	Application* CreateApplication();
}