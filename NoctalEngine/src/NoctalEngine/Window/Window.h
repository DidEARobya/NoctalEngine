#pragma once
#include "NoctalEngine/Core.h"

struct SDL_Window;

namespace NoctalEngine
{
	class Event;

	struct WindowProperties
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProperties(const std::string& title = "Noctal Engine", unsigned int width = 1280, unsigned int height = 720) : Title(title), Width(width), Height(height) {}
	};

	class NOCTAL_ENGINE_API Window
	{
	public:
		virtual ~Window() {};

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetVSync(bool enable) = 0;
		virtual bool IsVSync() const = 0;

		virtual void SetEventCallback(const std::function<void(Event&)> callback) = 0;

		virtual SDL_Window* GetSDLWindow() const = 0;
		static Window* Create(const WindowProperties& props = WindowProperties());

	public:
		struct WindowData
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;
			std::function<void(Event&)> eventCallback;
		};

		WindowData WindowData;

	protected:
		SDL_Window* m_Window;
	};
}