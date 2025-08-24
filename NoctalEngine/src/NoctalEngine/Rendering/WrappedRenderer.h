#pragma once

#include "NoctalEngine/Window/Window.h"
#include "NoctalEngine/Rendering/Bindable.h"

class Renderer;
struct SDL_Window;

class WrappedRenderer
{
public:
    WrappedRenderer() = default;
    virtual ~WrappedRenderer() = default;

    virtual void Init(const NoctalEngine::Window* windowRef) = 0;
    virtual void Destroy() = 0;

    virtual void BeginRender() = 0;
    virtual void Render() = 0;
    virtual void EndRender() = 0;

    virtual void OnWindowResize(const uint32_t width, const uint32_t height) = 0;

protected:
    SDL_Window* m_Window = nullptr;
    std::vector<std::unique_ptr<NoctalEngine::Bindable>> m_Bindables;
};
