#pragma once
#include "NoctalEngine/Core.h"
#include <SDL3/SDL.h>

namespace NoctalEngine
{
	class Event;
	class OpenGLWindow;

	enum TestGraphicsAPI
	{
		SDL,
		OPEN_GL
	};

	struct WindowProperties
	{
		TestGraphicsAPI TestAPI;

		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProperties(TestGraphicsAPI testApi = TestGraphicsAPI::OPEN_GL, const std::string& title = "Noctal Engine", uint32_t width = 1280, uint32_t height = 720) : TestAPI(testApi), Title(title), Width(width), Height(height) {}
	};

	class Window
	{
	public:
		Window() = default;
		virtual ~Window() {};

		virtual void OnUpdate() = 0;

		int GetWidth() const { return WindowData.Width; };
		int GetHeight() const { return WindowData.Height; };

		void UpdateSize() { SDL_GetWindowSize(m_Window, (int*)&WindowData.Width, (int*)&WindowData.Height); };

		virtual void SetVSync(bool enable) = 0;
		virtual bool IsVSync() const = 0;

		void SetEventCallback(const std::function<void(Event&)> callback) { WindowData.eventCallback = callback; };

		virtual SDL_Window* GetSDLWindow() const = 0;

		static Window* Create(const WindowProperties& props = WindowProperties());

	protected:
		virtual void Init(const WindowProperties& properties) = 0;
		virtual void Shutdown() = 0;

	public:
		struct WindowData
		{
			std::string Title;
			uint32_t Width = 0;
			uint32_t Height = 0;
			std::function<void(Event&)> eventCallback;
		};

		WindowData WindowData;

	protected:
		SDL_Window* m_Window = nullptr;
	};
}