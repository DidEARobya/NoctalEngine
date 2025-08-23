#pragma once
#include "NoctalEngine/Core.h"

struct SDL_Window;

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
		unsigned int Width;
		unsigned int Height;

		WindowProperties(TestGraphicsAPI testApi = TestGraphicsAPI::OPEN_GL, const std::string& title = "Noctal Engine", unsigned int width = 1280, unsigned int height = 720) : TestAPI(testApi), Title(title), Width(width), Height(height) {}
	};

	class NOCTAL_ENGINE_API Window
	{
	public:
		Window() = default;
		virtual ~Window() {};

		virtual void OnUpdate() = 0;

		unsigned int GetWidth() const { return WindowData.Width; };
		unsigned int GetHeight() const { return WindowData.Height; };

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
			unsigned int Width = 0;
			unsigned int Height = 0;
			std::function<void(Event&)> eventCallback;
		};

		WindowData WindowData;

	protected:
		SDL_Window* m_Window = nullptr;
	};
}