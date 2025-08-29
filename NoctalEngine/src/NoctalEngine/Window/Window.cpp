#include "nepch.h"
#include "Window.h"
#include "OpenGL/OpenGLWindow.h"

namespace NoctalEngine
{
	Window* Window::Create(const WindowProperties& props)
	{
		switch (props.TestAPI)
		{
		case TestGraphicsAPI::OPEN_GL:
			return new OpenGLWindow(props);

		default:
			NE_ENGINE_ASSERT(true, "No valid Window available for current Graphics API");
			return nullptr;
		}
	}
}