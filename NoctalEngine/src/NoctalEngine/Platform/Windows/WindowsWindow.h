#pragma once
#include "NoctalEngine/Window/Window.h"
struct SDL_Window;

namespace NoctalEngine
{

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProperties& properties);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_WindowData.Width; }
		inline unsigned int GetHeight() const override { return m_WindowData.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_WindowData.EventCallback = callback; }
		void SetVSync(bool enabled);
		bool IsVSync() const;

		SDL_Window* GetSDLWindow() const override { return m_Window; };
	private:
		virtual void Init(const WindowProperties& properties);
		virtual void Shutdown();

	private:
		struct WindowData
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;
			EventCallbackFn EventCallback;
		};

		WindowData m_WindowData;
	};
}