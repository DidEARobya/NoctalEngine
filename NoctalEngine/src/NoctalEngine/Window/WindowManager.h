#pragma once
#include "Window.h"

namespace NoctalEngine
{
	class WindowManager
	{
	public:
		static void InitWindowAPI();
		static void TerminateWindowAPI();

		static Window* CreateWindowInternal(const WindowProperties& properties = WindowProperties());
		static void DestroyWindow(const Window* window);

	private:
		static bool m_Initialised;
	};
}