#pragma once
#include "NoctalEngine/Window/Window.h"

struct SDL_Window;

namespace NoctalEngine
{
	// SDL window for the D3D12 renderer.
	// Unlike OpenGL, there's no SDL_WINDOW_OPENGL flag or GL context: D3D12 only needs the
	// native window handle, which the renderer grabs from SDL itself.
	// VSync is a swap chain setting in D3D12, so the window just remembers the choice and
	// the renderer reads it when presenting.
	class D3D12Window : public Window
	{
	public:
		D3D12Window(const WindowProperties& properties);
		virtual ~D3D12Window() override;

		void OnUpdate() override;

		void SetVSync(bool enabled) override { m_VSync = enabled; }
		bool IsVSync() const override { return m_VSync; }

		SDL_Window* GetSDLWindow() const override { return m_Window; };

	protected:
		virtual void Init(const WindowProperties& properties) override;
		virtual void Shutdown() override;

	private:
		bool m_VSync = true;
	};
}
