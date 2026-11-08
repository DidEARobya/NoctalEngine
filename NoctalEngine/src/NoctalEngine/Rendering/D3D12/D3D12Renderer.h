#pragma once
#include "NoctalEngine/Rendering/WrappedRenderer.h"
#include "NoctalEngine/Rendering/RendererConfig.h"
#include "D3D12Common.h"
#include "Descriptors/D3D12DescriptorHeap.h"
#include <array>
#include <functional>

// ---------------------------------------------------------------------------------
// D3D12Renderer
// The Direct3D 12 backend. It owns the device, the swap chain (the images shown on screen),
// the command list we record draws into, and the CPU/GPU synchronisation.
//
// The big difference from OpenGL: D3D12 does almost nothing for you.
//   - You record commands into a command list, then submit the whole list at once.
//   - The CPU and GPU run at the same time, so YOU must make sure the CPU doesn't overwrite
//     or delete something the GPU is still using. That's what the fence + "frames in flight"
//     + deferred release code below is for.
//
// The D3D12 resource classes (buffers, textures, pipeline) reach the device and command
// list through D3D12Renderer::Get().
// ---------------------------------------------------------------------------------
class D3D12Renderer : public WrappedRenderer
{
public:
    D3D12Renderer() = default;
    virtual ~D3D12Renderer() override;

    // ---- WrappedRenderer resource creation ----
    virtual std::shared_ptr<NoctalEngine::Texture> CreateTexture(const std::string& path) override;
    virtual std::shared_ptr<NoctalEngine::Texture> CreateTexture(uint32_t width, uint32_t height, const void* rgbaPixels) override;
    virtual std::unique_ptr<NoctalEngine::VertexBuffer> CreateVertexBuffer(uint32_t maxBytesPerFrame, const NoctalEngine::BufferLayout& layout) override;
    virtual std::unique_ptr<NoctalEngine::IndexBuffer> CreateIndexBuffer(const void* indices, uint32_t count, NoctalEngine::IndexFormat format) override;
    virtual std::unique_ptr<NoctalEngine::ConstantBuffer> CreateConstantBuffer(uint32_t size, uint32_t bindSlot) override;
    virtual std::unique_ptr<NoctalEngine::Pipeline> CreatePipeline(const NoctalEngine::PipelineSpecification& specification) override;

    virtual void DrawIndexed(uint32_t indexCount) override;

    // ---- Used by the D3D12 resource classes ----
    static D3D12Renderer& Get() { NE_ENGINE_ASSERT(s_Instance, "D3D12Renderer doesn't exist"); return *s_Instance; }
    // False before Init() and after Destroy(). Resources check this in their destructors.
    static bool IsActive() { return s_Instance != nullptr && s_Instance->m_Initialised; }

    ID3D12Device* GetDevice() const { return m_Device.Get(); }
    ID3D12GraphicsCommandList* GetCommandList() const { return m_CommandList.Get(); }
    ID3D12RootSignature* GetRootSignature() const { return m_RootSignature.Get(); }

    // Which of the FramesInFlight slots the CPU is recording into right now (0..FramesInFlight-1).
    uint32_t GetFrameIndex() const { return m_FrameIndex; }
    // Ever-increasing frame counter. Used by per-frame allocators to know when to reset.
    uint64_t GetFrameNumber() const { return m_FrameNumber; }

    // Creates a buffer resource. Upload heap buffers start in GENERIC_READ, others in COMMON.
    ComPtr<ID3D12Resource> CreateBuffer(uint64_t size, D3D12_HEAP_TYPE heapType, const wchar_t* debugName);

    // Records some commands, runs them on the GPU straight away and waits for them to finish.
    // Simple way to upload data at load time (textures, static buffers).
    void ExecuteImmediate(const std::function<void(ID3D12GraphicsCommandList*)>& recordCommands);

    // Keeps a GPU object alive until the GPU has definitely finished with it.
    void DeferRelease(ComPtr<IUnknown> object);

    // Staging (CPU-only) descriptors, used by textures for their SRV.
    uint32_t AllocateStagingDescriptor() { return m_StagingAllocator.Allocate(); }
    void FreeStagingDescriptor(uint32_t index) { m_StagingAllocator.Free(index); }
    D3D12_CPU_DESCRIPTOR_HANDLE GetStagingCPUHandle(uint32_t index) const { return m_StagingHeap.GetCPUHandle(index); }

    // Called by D3D12Texture2D::Bind(): "put this texture in slot N for the next draw".
    void SetTextureSlot(uint32_t slot, D3D12_CPU_DESCRIPTOR_HANDLE srv);

    // Root parameter indices (must match CreateRootSignature and the HLSL registers).
    static constexpr uint32_t RootParam_CameraCBV = 0;  // b0
    static constexpr uint32_t RootParam_Textures  = 1;  // t0 .. t(MaxTextureSlots-1)

protected:
    // ---- WrappedRenderer lifetime / frame ----
    virtual void Init(const NoctalEngine::Window* windowRef) override;
    virtual void Destroy() override;

    virtual void BeginFrame() override;
    virtual void EndFrame() override;

    virtual void OnWindowResize(const uint32_t width, const uint32_t height) override;

    virtual const char* GetVendor() override { return m_VendorName.c_str(); }
    virtual const char* GetRenderer() override { return m_RendererName.c_str(); }
    virtual const char* GetVersion() override { return m_VersionName.c_str(); }

    virtual std::shared_ptr<NoctalEngine::Shader> CreateShader(const std::string& filePath) override;

private:
    void EnableDebugLayer();
    void CreateDevice();
    void CreateSwapChain(HWND hwnd);
    void CreateRenderTargets();
    void CreateCommandObjects();
    void CreateDescriptorHeaps();
    void CreateRootSignature();
    void InitImGui();

    // CPU waits until the GPU reaches 'fenceValue'.
    void WaitForFenceValue(uint64_t fenceValue);
    // CPU waits until the GPU has finished ALL submitted work.
    void WaitForGpu();
    // Signals the end of this frame and waits until the next frame slot is free.
    void MoveToNextFrame();

private:
    inline static D3D12Renderer* s_Instance = nullptr;
    bool m_Initialised = false;

    // ---- Core objects ----
    ComPtr<IDXGIFactory4> m_Factory;
    ComPtr<IDXGIAdapter1> m_Adapter;            // The physical GPU
    ComPtr<ID3D12Device> m_Device;              // Creates everything
    ComPtr<ID3D12CommandQueue> m_CommandQueue;  // Where command lists are submitted
    ComPtr<IDXGISwapChain3> m_SwapChain;        // The images we draw into and show on screen

    // ---- Back buffers ----
    std::array<ComPtr<ID3D12Resource>, D3D12Config::FramesInFlight> m_BackBuffers;
    D3D12DescriptorHeap m_RTVHeap;              // Render Target Views for the back buffers

    // ---- Command recording ----
    // One allocator per frame: an allocator owns the memory of recorded commands, so it can
    // only be reset once the GPU has finished that frame.
    std::array<ComPtr<ID3D12CommandAllocator>, D3D12Config::FramesInFlight> m_CommandAllocators;
    ComPtr<ID3D12GraphicsCommandList> m_CommandList;

    // Separate allocator/list for ExecuteImmediate uploads.
    ComPtr<ID3D12CommandAllocator> m_UploadAllocator;
    ComPtr<ID3D12GraphicsCommandList> m_UploadCommandList;

    // ---- CPU/GPU synchronisation ----
    // A fence is a counter the GPU sets when it reaches a point in the queue.
    // We remember the value for each frame, and before reusing that frame's resources we
    // wait until the GPU has passed it.
    ComPtr<ID3D12Fence> m_Fence;
    HANDLE m_FenceEvent = nullptr;
    uint64_t m_FenceValue = 0;
    std::array<uint64_t, D3D12Config::FramesInFlight> m_FrameFenceValues{};

    uint32_t m_FrameIndex = 0;
    uint64_t m_FrameNumber = 0;

    // Objects waiting to be released once their frame has finished on the GPU.
    std::array<std::vector<ComPtr<IUnknown>>, D3D12Config::FramesInFlight> m_DeferredReleases;

    // ---- Descriptors ----
    // Staging heap (CPU only): every texture's SRV lives here permanently.
    D3D12DescriptorHeap m_StagingHeap;
    D3D12DescriptorAllocator m_StagingAllocator;
    uint32_t m_NullSRVIndex = D3D12DescriptorAllocator::InvalidIndex;

    // Shader-visible heap, split into:
    //   [0 .. ImGuiDescriptorCount)                      long-lived, handed to ImGui
    //   then FramesInFlight blocks of PerFrameDescriptorCount   texture tables for batches,
    //                                                     reset every frame (linear allocation)
    D3D12DescriptorHeap m_ShaderHeap;
    D3D12DescriptorAllocator m_ImGuiDescriptors;
    uint32_t m_FrameDescriptorOffset = 0;

    // The textures the next draw will use. Copied into the shader-visible heap on DrawIndexed,
    // but only if something changed since the last draw.
    std::array<D3D12_CPU_DESCRIPTOR_HANDLE, NoctalEngine::RendererConfig::MaxTextureSlots> m_PendingTextureSlots{};
    bool m_TextureSlotsDirty = true;

    // ---- Pipeline layout ----
    // The root signature is the "function signature" of our shaders: what data they take
    // (one constant buffer + a table of textures + one sampler). Shared by every 2D pipeline.
    ComPtr<ID3D12RootSignature> m_RootSignature;

    // ---- Misc ----
    uint32_t m_Width = 0;
    uint32_t m_Height = 0;
    bool m_TearingSupported = false;    // Needed to present with VSync off on flip-model swap chains.

    std::string m_VendorName = "Unknown";
    std::string m_RendererName = "Unknown";
    std::string m_VersionName = "Direct3D 12";
};
