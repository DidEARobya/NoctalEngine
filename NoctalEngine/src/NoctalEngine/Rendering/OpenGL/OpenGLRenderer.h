#pragma once
#include "NoctalEngine/Rendering/WrappedRenderer.h"
#include "GLAD/include/glad/glad.h"
#include "NoctalEngine/Window/Window.h"
#include <SDL3/SDL.h>

class OpenGLRenderer : public WrappedRenderer
{
public:
    OpenGLRenderer() = default;
    ~OpenGLRenderer() = default;

    virtual void Init(const NoctalEngine::Window* windowRef) override;
    virtual void Destroy() override;

    virtual void BeginRender() override;
    virtual void Render() override;
    virtual void EndRender() override;

    virtual void OnWindowResize(const uint32_t width, const uint32_t height) override;

private:
    SDL_GLContext m_GLContext = nullptr;
    unsigned int m_IndexBuffer;
    unsigned int m_VertexArray;
    unsigned int m_VertexBuffer;
};
