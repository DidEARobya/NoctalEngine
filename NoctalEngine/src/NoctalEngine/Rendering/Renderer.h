#pragma once
#include "WrappedRenderer.h"

namespace NoctalEngine
{
    class Window;

    class NOCTAL_ENGINE_API Renderer
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
    protected:
        static std::unique_ptr<Renderer> m_Instance;
        std::unique_ptr<WrappedRenderer> m_WrappedRenderer;
    };

}