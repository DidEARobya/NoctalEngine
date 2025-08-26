#pragma once
#include "WrappedRenderer.h"
#include "Geometry/Geometry.h"

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

        static Renderer& Instance() { NE_ENGINE_ASSERT(s_Instance, "Renderer is invalid"); return *s_Instance; }

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

        std::shared_ptr<Drawable> CreateDrawable(Geometry geometry);

        void SetIndexBuffer(IndexBuffer* indexBuffer);
        void DrawIndexed();

        const glm::mat4& GetCameraViewProjectionMatrix() const;

    protected:
        static std::unique_ptr<Renderer> s_Instance;
        std::unique_ptr<WrappedRenderer> m_WrappedRenderer = nullptr;
    };

}