#pragma once
#include "NoctalEngine/Rendering/Bindable.h"
#include "NoctalEngine/Rendering/Shaders/ShaderLibrary.h"
#include "NoctalEngine/Rendering/Buffers/IndexBuffer.h"
#include "NoctalEngine/Rendering/Buffers/BufferLayout.h"

namespace NoctalEngine
{
    class Window;
    class Renderer;
    class Shader;
    class Texture;
    class VertexBuffer;
    class IndexBuffer;
    class ConstantBuffer;
    class Pipeline;
    struct PipelineSpecification;
}

struct SDL_Window;

// ---------------------------------------------------------------------------------
// WrappedRenderer
// The interface every graphics API backend implements (D3D12Renderer today).
//
// The split is:
//   - Renderer (the public facade) + BatchRenderer2D: API-agnostic. Culling, layer sorting,
//     batching, stats. Written once, shared by every backend.
//   - WrappedRenderer subclasses: the API-specific bits. Starting/ending a frame, presenting,
//     creating GPU resources, and issuing the actual draw call.
//
// So adding a new API means implementing this class plus the resource classes
// (VertexBuffer, IndexBuffer, ConstantBuffer, Texture, Shader, Pipeline). No engine code changes.
// ---------------------------------------------------------------------------------
class WrappedRenderer
{
public:
    virtual ~WrappedRenderer() = default;

    // ---- Resource creation (used by the API-agnostic BatchRenderer2D) ----

    virtual std::shared_ptr<NoctalEngine::Texture> CreateTexture(const std::string& path) = 0;
    // Create a texture from raw RGBA8 pixels (4 bytes per pixel).
    virtual std::shared_ptr<NoctalEngine::Texture> CreateTexture(uint32_t width, uint32_t height, const void* rgbaPixels) = 0;

    // 'maxBytesPerFrame' is the most data that will be written with SetData() in one frame.
    virtual std::unique_ptr<NoctalEngine::VertexBuffer> CreateVertexBuffer(uint32_t maxBytesPerFrame, const NoctalEngine::BufferLayout& layout) = 0;
    virtual std::unique_ptr<NoctalEngine::IndexBuffer> CreateIndexBuffer(const void* indices, uint32_t count, NoctalEngine::IndexFormat format) = 0;
    // 'bindSlot' is the shader register (b0, b1, ... in HLSL).
    virtual std::unique_ptr<NoctalEngine::ConstantBuffer> CreateConstantBuffer(uint32_t size, uint32_t bindSlot) = 0;
    virtual std::unique_ptr<NoctalEngine::Pipeline> CreatePipeline(const NoctalEngine::PipelineSpecification& specification) = 0;

    // ---- Drawing ----

    // Draws 'indexCount' indices using whatever pipeline/buffers/textures are currently bound.
    virtual void DrawIndexed(uint32_t indexCount) = 0;

    // ---- Shaders ----

    std::shared_ptr<NoctalEngine::Shader> GetShader(const std::string& shaderName)
    {
        NE_ENGINE_ASSERT(m_ShaderLibrary, "Shader Library doesn't exist");
        return m_ShaderLibrary->GetShader(shaderName);
    }

protected:
    friend class NoctalEngine::Renderer;

    WrappedRenderer() = default;

    // ---- Lifetime ----
    virtual void Init(const NoctalEngine::Window* windowRef) = 0;
    virtual void Destroy() = 0;

    // ---- Frame ----
    // BeginFrame: get ready to record draws (clear the screen, start the ImGui frame).
    // EndFrame:   draw ImGui, submit all recorded work to the GPU and present.
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;

    virtual void OnWindowResize(const uint32_t width, const uint32_t height) = 0;

    // ---- Info (shown in the ImGui "Renderer" window) ----
    virtual const char* GetVendor() = 0;
    virtual const char* GetRenderer() = 0;
    virtual const char* GetVersion() = 0;

    // Compiles/loads a single shader stage from disk. Called by the ShaderLibrary.
    virtual std::shared_ptr<NoctalEngine::Shader> CreateShader(const std::string& filePath) = 0;

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

protected:
    SDL_Window* m_Window = nullptr;
    std::unique_ptr<NoctalEngine::ShaderLibrary> m_ShaderLibrary;
};
