#pragma once

#include "NoctalEngine/Window/Window.h"

class Renderer;
struct SDL_Window;

class WrappedRenderer
{
public:
    WrappedRenderer() = default;
    virtual ~WrappedRenderer() = default;

    static WrappedRenderer& Instance() { return *m_Instance; }

    virtual void Init(const NoctalEngine::Window* windowRef) = 0;
    virtual void Destroy() = 0;

    virtual void BeginRender() = 0;
    virtual void Render() = 0;
    virtual void EndRender() = 0;

    virtual void OnWindowResize(const uint32_t width, const uint32_t height) = 0;
protected:
    static std::unique_ptr<WrappedRenderer> m_Instance;
};
