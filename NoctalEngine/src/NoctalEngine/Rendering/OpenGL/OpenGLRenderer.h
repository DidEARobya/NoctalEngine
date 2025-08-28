#pragma once
#include "NoctalEngine/Rendering/WrappedRenderer.h"

#include <SDL3/SDL.h>

class OpenGLRenderer : public WrappedRenderer
{
public:
    OpenGLRenderer() = default;
    virtual ~OpenGLRenderer() override = default;

    virtual void Init(const NoctalEngine::Window* windowRef) override;
    virtual void Destroy() override;

    virtual void BeginRender() override;
    virtual void Render() override;
    virtual void EndRender() override;

    virtual void OnWindowResize(const uint32_t width, const uint32_t height) override;

    virtual const char* GetVendor() override;
    virtual const char* GetRenderer() override;
    virtual const char* GetVersion() override;

    virtual std::shared_ptr<NoctalEngine::Shader> GetShader(const std::string& shaderName) override;
    virtual std::shared_ptr<NoctalEngine::Shader> CreateShader(const std::string& filePath) override;

    virtual NoctalEngine::VertexBuffer* CreateVertexBuffer(float* vertices, uint32_t size, const NoctalEngine::BufferLayout& layout) override;
    virtual NoctalEngine::IndexBuffer* CreateIndexBuffer(uint32_t* indices, uint32_t size) override;

    virtual std::shared_ptr<Drawable>CreateDrawable(NoctalEngine::Geometry geometry) override;
    virtual std::shared_ptr<NoctalEngine::Texture> CreateTexture(const std::string& path) override;

    virtual void SetIndexBuffer(NoctalEngine::IndexBuffer* indexBuffer) override { m_IndexBuffer = indexBuffer; };
    virtual void DrawIndexed() override;

protected:

private:
    NoctalEngine::IndexBuffer* m_IndexBuffer = nullptr;
    SDL_GLContext m_GLContext = nullptr;
};
