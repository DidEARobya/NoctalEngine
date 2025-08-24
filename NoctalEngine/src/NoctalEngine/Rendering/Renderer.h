#pragma once
#include "WrappedRenderer.h"

namespace NoctalEngine
{
    class Window;
    class Shader;
    class VertexBuffer;
    class IndexBuffer;
    class BufferLayout;

    class Renderer
    {
    public:
        Renderer() = default;
        ~Renderer() = default;

        static Renderer& Instance() { return *m_Instance; }

        void Init(const Window* windowRef);
        void Destroy();

        void BeginRender();
        void Render();
        void EndRender();

        void OnWindowResize(const uint32_t width, const uint32_t height);

        const char* GetVendor();
        const char* GetRenderer();
        const char* GetVersion();

        Shader* CreateShader(const std::string& vertexSource, const std::string& pixelSource);
        VertexBuffer* CreateVertexBuffer(float* vertices, uint32_t size, const BufferLayout& layout);
        IndexBuffer* CreateIndexBuffer(uint32_t* indices, uint32_t size);

    protected:
        static std::unique_ptr<Renderer> m_Instance;
        std::unique_ptr<WrappedRenderer> m_WrappedRenderer = nullptr;
    };

}