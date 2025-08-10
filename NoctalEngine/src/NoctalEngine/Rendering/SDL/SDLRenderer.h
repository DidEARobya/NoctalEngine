#pragma once
#include "NoctalEngine/Rendering/WrappedRenderer.h"
#include "NoctalEngine/Window/Window.h"

struct SDL_Renderer;

class NOCTAL_ENGINE_API SDLRenderer : public WrappedRenderer
{
public:
    SDLRenderer() = default;
    ~SDLRenderer() = default;

    virtual void Init(const NoctalEngine::Window* windowRef) override;
    virtual void Destroy() override;

    virtual void BeginRender() override;
    virtual void Render() override;
    virtual void EndRender() override;

    virtual void OnWindowResize(const uint32_t width, const uint32_t height) override;
private:
    SDL_Renderer* m_SDLRenderer = nullptr;
};

