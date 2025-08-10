#pragma once
#include "NoctalEngine/Window/Window.h"
struct SDL_Window;

namespace NoctalEngine
{

	class NOCTAL_ENGINE_API WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProperties& properties);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return WindowData.Width; }
		inline unsigned int GetHeight() const override { return WindowData.Height; }

		virtual void SetEventCallback(const std::function<void(Event&)> callback) { WindowData.eventCallback = callback; };

		void SetVSync(bool enabled);
		bool IsVSync() const;

		SDL_Window* GetSDLWindow() const override { return m_Window; };
	private:
		virtual void Init(const WindowProperties& properties);
		virtual void Shutdown();
	};
}