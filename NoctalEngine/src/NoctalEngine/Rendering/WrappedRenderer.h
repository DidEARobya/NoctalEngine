#pragma once
#include "NoctalEngine/Rendering/Bindable.h"
#include "NoctalEngine/Rendering/Drawables/Drawable.h"
#include "NoctalEngine/Rendering/Shaders/ShaderLibrary.h"

namespace NoctalEngine
{
    class Window;
    class Renderer;
    class Shader;
    class VertexBuffer;
    class IndexBuffer;
    class Texture;
    class BufferLayout;
}

struct SDL_Window;

class WrappedRenderer
{
public:
    virtual ~WrappedRenderer() = default;

protected:
    friend class NoctalEngine::Renderer;

    WrappedRenderer() = default;

    virtual void Init(const NoctalEngine::Window* windowRef) = 0;
    virtual void Destroy() = 0;

    virtual void BeginRender() = 0;
    virtual void Render() = 0;
    virtual void EndRender() = 0;

    virtual void OnWindowResize(const uint32_t width, const uint32_t height) = 0;

    virtual const char* GetVendor() = 0;
    virtual const char* GetRenderer() = 0;
    virtual const char* GetVersion() = 0;

    std::shared_ptr<NoctalEngine::Shader> LoadShader(const std::string& filePath)
    {
        NE_ENGINE_ASSERT(m_ShaderLibrary, "Shader Library doesn't exist");
        return m_ShaderLibrary->LoadShader(filePath);
    }

    std::shared_ptr<NoctalEngine::Shader> LoadShader(const std::string& customName, const std::string& filePath)
    {
        NE_ENGINE_ASSERT(m_ShaderLibrary, "Shader Library doesn't exist");
        return m_ShaderLibrary->LoadShader(customName, filePath);
    }

    virtual std::shared_ptr<NoctalEngine::Shader> GetShader(const std::string& shaderName) = 0;
    virtual std::shared_ptr<NoctalEngine::Shader> CreateShader(const std::string& filePath) = 0;

    virtual std::shared_ptr<Drawable> CreateDrawable(glm::vec2 position = glm::vec2(0.0f), glm::vec2 scale = glm::vec2(1.0f)) = 0;
    virtual std::shared_ptr<NoctalEngine::Texture> CreateTexture(const std::string& path) = 0;

    virtual void SetIndexBuffer(NoctalEngine::IndexBuffer* indexBuffer) = 0;
    virtual void DrawIndexed() = 0;
protected:
    SDL_Window* m_Window = nullptr;  
    std::vector<std::shared_ptr<Drawable>> m_Drawables;
    std::unique_ptr<NoctalEngine::ShaderLibrary> m_ShaderLibrary;
};
