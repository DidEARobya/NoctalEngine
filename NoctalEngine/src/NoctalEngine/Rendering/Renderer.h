#pragma once
#include "WrappedRenderer.h"
#include "Geometry/Geometry.h"
#include "RendererConfig.h"
#include "Drawables/Drawable.h"
#include "Batching/BatchRenderer2D.h"
#include <glm/glm.hpp>
#include <unordered_map>

namespace NoctalEngine
{
    class Window;
    class Shader;
    class Texture;

    // ---------------------------------------------------------------------------------
    // Renderer
    // The one class engine/game code talks to. It owns:
    //   - the API backend (WrappedRenderer, e.g. D3D12Renderer), chosen in Init()
    //   - the API-agnostic BatchRenderer2D that does culling, layers and batching
    //   - the list of registered Drawables
    // Nothing in here knows which graphics API is running.
    // ---------------------------------------------------------------------------------
    class Renderer
    {
    public:
        Renderer() = default;
        ~Renderer() = default;

        static Renderer& Instance() { NE_ENGINE_ASSERT(s_Instance, "Renderer is invalid"); return *s_Instance; }

        void Init(const Window* windowRef);
        void Destroy();

        // Called by Application every frame, in this order.
        void BeginRender();     // start of frame
        void Render();          // cull + sort + batch + draw everything
        void EndRender();       // ImGui + present

        void OnWindowResize(const uint32_t width, const uint32_t height);

        const char* GetVendor();
        const char* GetRenderer();
        const char* GetVersion();

        std::shared_ptr<Shader> LoadShader(const std::string& filePath);
        std::shared_ptr<Shader> LoadShader(const std::string& customName, const std::string& filePath);
        std::shared_ptr<Shader> GetShader(const std::string& shaderName);

        // Loading the same file twice returns the same texture (cached while anything still uses it).
        std::shared_ptr<Texture> CreateTexture(const std::string& path, const std::string& assetDir = ASSET_DIR);

        // Creates a drawable that's drawn every frame until you drop your last shared_ptr to it.
        std::shared_ptr<Drawable> CreateDrawable(Geometry type, glm::vec2 position = glm::vec2(0.0f), glm::vec2 scale = glm::vec2(1.0f));

        // ---- Immediate mode ----
        // Draw a quad for THIS frame only. Call it every frame you want it visible (e.g. from OnUpdate).
        // Batched together with everything else, so thousands of these are fine.
        void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& colour, int32_t layer = RenderLayer::Default, float rotation = 0.0f);
        void DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture, const glm::vec4& tint = glm::vec4(1.0f), int32_t layer = RenderLayer::Default, float rotation = 0.0f);

        // ---- Debug ----
        const RenderStats& GetStats() const;
        void SetCullingEnabled(bool enabled);
        bool IsCullingEnabled() const;

    protected:
        friend class ShaderLibrary;
        std::shared_ptr<Shader> CreateShader(const std::string& filePath);

    private:
        void DrawStatsWindow();

    protected:
        static std::unique_ptr<Renderer> s_Instance;
        std::unique_ptr<WrappedRenderer> m_WrappedRenderer = nullptr;
        std::unique_ptr<BatchRenderer2D> m_BatchRenderer = nullptr;

        // weak_ptr: the renderer doesn't keep drawables alive, the game does.
        std::vector<std::weak_ptr<Drawable>> m_Drawables;
        std::unordered_map<std::string, std::weak_ptr<Texture>> m_TextureCache;
    };
}
