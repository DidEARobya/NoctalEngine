#pragma once
#include "NoctalEngine/Rendering/WrappedRenderer.h"
#include "NoctalEngine/Rendering/OpenGL/Buffers/OpenGLUniformBufferObject.h"
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

    virtual std::shared_ptr<Drawable>CreateDrawable(NoctalEngine::Geometry type, glm::vec2 position = glm::vec2(0.0f), glm::vec2 scale = glm::vec2(1.0f)) override;
    virtual std::shared_ptr<NoctalEngine::Texture> CreateTexture(const std::string& path) override;

    virtual void DrawIndexed() override;

    virtual void SubmitVertexArray(NoctalEngine::Geometry geometry, std::unique_ptr<NoctalEngine::VertexArray> vertexArray) override;

    void DrawIndexed(NoctalEngine::VertexArray* vertexArray);

private:
    void StartBatch();
    void NextBatch();
    void Flush();

private:
    SDL_GLContext m_GLContext = nullptr;

    std::unique_ptr<OpenGLFrameUniformBufferObject> m_FrameBuffer = nullptr;
    FrameData m_FrameData;

    //std::shared_ptr<OpenGLObjectUniformBufferObject> m_ObjectBuffer = nullptr;
};
