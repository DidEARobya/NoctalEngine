#pragma once
#include "NoctalEngine/Window/Window.h"

struct SDL_Window;

namespace NoctalEngine
{
	class NOCTAL_ENGINE_API OpenGLWindow : public Window
	{
	public:
		OpenGLWindow(const WindowProperties& properties);
		virtual ~OpenGLWindow() override;

		void OnUpdate() override;

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		SDL_Window* GetSDLWindow() const override { return m_Window; };
	protected:
		virtual void Init(const WindowProperties& properties) override;
		virtual void Shutdown() override;
	};
}