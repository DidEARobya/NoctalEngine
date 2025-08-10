#pragma once
#include "NoctalEngine/Core.h"

namespace NoctalEngine
{
	class Window;

	class NOCTAL_ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Window> m_Window;
		bool m_AppRunning = true;
	};

	Application* CreateApplication();
}