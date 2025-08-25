#pragma once

#include "NoctalEngine/Window/Window.h"
#include "NoctalEngine/Rendering/Bindable.h"
#include "NoctalEngine/Rendering/Shader.h"
#include "NoctalEngine/Rendering/VertexBuffer.h"
#include "NoctalEngine/Rendering/IndexBuffer.h"
#include "NoctalEngine/Rendering/BufferLayout.h"
#include "NoctalEngine/Rendering/Drawables/Drawable.h"

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

    virtual NoctalEngine::Shader* CreateShader(const std::string& vertexSource, const std::string& pixelSource) = 0;
    virtual NoctalEngine::VertexBuffer* CreateVertexBuffer(float* vertices, uint32_t size, const NoctalEngine::BufferLayout& layout) = 0;
    virtual NoctalEngine::IndexBuffer* CreateIndexBuffer(uint32_t* indices, uint32_t size) = 0;

    virtual void SetIndexBuffer(NoctalEngine::IndexBuffer* indexBuffer) = 0;
    virtual void DrawIndexed() = 0;

protected:
    SDL_Window* m_Window = nullptr;  
    std::vector<std::unique_ptr<Drawable>> m_Drawables;
};
