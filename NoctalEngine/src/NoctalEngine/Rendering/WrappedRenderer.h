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

    virtual const char* GetVendor() = 0;
    virtual const char* GetRenderer() = 0;
    virtual const char* GetVersion() = 0;

protected:
    SDL_Window* m_Window = nullptr;
    std::vector<std::unique_ptr<Bindable>> m_Bindables;
};
