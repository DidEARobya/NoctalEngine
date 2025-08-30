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
    class Texture;

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

        std::shared_ptr<Shader> LoadShader(const std::string& filePath);
        std::shared_ptr<Shader> LoadShader(const std::string& customName, const std::string& filePath);
        std::shared_ptr<Shader> GetShader(const std::string& shaderName);

        std::shared_ptr<Texture> CreateTexture(const std::string& path, const std::string& assetDir = ASSET_DIR);
        std::shared_ptr<Drawable> CreateDrawable(Geometry geometry);

        void SetIndexBuffer(IndexBuffer* indexBuffer);
        void DrawIndexed();

    protected:
        friend class ShaderLibrary;
        std::shared_ptr<Shader> CreateShader(const std::string& filePath);

    protected:
        static std::unique_ptr<Renderer> s_Instance;
        std::unique_ptr<WrappedRenderer> m_WrappedRenderer = nullptr;
    };

}