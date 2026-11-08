#include "nepch.h"
#include "Window.h"
#include "OpenGL/OpenGLWindow.h"
#include "D3D12/D3D12Window.h"

namespace NoctalEngine
{
	Window* Window::Create(const WindowProperties& props)
	{
		switch (props.TestAPI)
		{
		case TestGraphicsAPI::OPEN_GL:
			return new OpenGLWindow(props);

		case TestGraphicsAPI::D3D12:
			return new D3D12Window(props);

		default:
			NE_ENGINE_ASSERT(true, "No valid Window available for current Graphics API");
			return nullptr;
		}
	}
}