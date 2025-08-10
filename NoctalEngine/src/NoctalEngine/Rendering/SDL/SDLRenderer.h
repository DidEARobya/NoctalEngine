#pragma once
#include "NoctalEngine/Rendering/WrappedRenderer.h"
#include "NoctalEngine/Window/Window.h"

struct SDL_Renderer;

class SDLRenderer : public WrappedRenderer
{
public:
    SDLRenderer() = default;
    ~SDLRenderer() override;

    virtual void Init(const NoctalEngine::Window* windowRef) override;
    virtual void Destroy() override;

    virtual void BeginRender() override;
    virtual void Render() override;
    virtual void EndRender() override;

    virtual void OnWindowResize(const uint32_t width, const uint32_t height) override;
protected:
    static std::unique_ptr<Renderer> m_Instance;
    const NoctalEngine::Window* m_WindowRef;

private:
    SDL_Renderer* m_SDLRenderer = nullptr;
};

