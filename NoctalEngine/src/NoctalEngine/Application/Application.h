#pragma once
#include "NoctalEngine/Core.h"

namespace NoctalEngine
{
	class Window;
	class Event;

	class NOCTAL_ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& event);

	private:
		Window* m_Window = nullptr;
		bool m_AppRunning = true;
	};

	Application* CreateApplication();
}