#include "nepch.h"
#include "D3D12Renderer.h"
#include "NoctalEngine/Window/Window.h"
#include "NoctalEngine/Application/Application.h"
#include "NoctalEngine/Rendering/Pipeline/Pipeline.h"

#include "Buffers/D3D12VertexBuffer.h"
#include "Buffers/D3D12IndexBuffer.h"
#include "Buffers/D3D12ConstantBuffer.h"
#include "Shaders/D3D12Shader.h"
#include "Pipeline/D3D12Pipeline.h"
#include "Textures/D3D12Texture2D.h"

#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_dx12.h>

//----- LIFETIME -----
D3D12Renderer::~D3D12Renderer()
{
	// Normally Destroy() has already run. This is just a safety net.
	if (m_Initialised)
	{
		Destroy();
	}
}

void D3D12Renderer::Init(const NoctalEngine::Window* windowRef)
{
	NE_ENGINE_ASSERT(s_Instance == nullptr, "Only one D3D12Renderer can exist");
	s_Instance = this;

	m_Window = windowRef->GetSDLWindow();
	NE_ENGINE_SDL_ASSERT(m_Window);

	m_Width = windowRef->GetWidth();
	m_Height = windowRef->GetHeight();

	// SDL created the window; D3D needs its native Win32 handle (HWND).
	HWND hwnd = (HWND)SDL_GetPointerProperty(SDL_GetWindowProperties(m_Window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
	NE_ENGINE_ASSERT(hwnd, "Failed to get the Win32 window handle from SDL");

	EnableDebugLayer();
	CreateDevice();
	CreateCommandObjects();
	CreateSwapChain(hwnd);
	CreateDescriptorHeaps();
	CreateRenderTargets();
	CreateRootSignature();

	m_Initialised = true;

	InitImGui();

	m_ShaderLibrary = std::make_unique<NoctalEngine::ShaderLibrary>();
	m_ShaderLibrary->LoadShader(ASSET_DIR "Shaders/D3D12/Batch2DVS.hlsl", false);
	m_ShaderLibrary->LoadShader(ASSET_DIR "Shaders/D3D12/Batch2DPS.hlsl", false);
	m_ShaderLibrary->SortShaders();

	NE_ENGINE_INFO("D3D12 initialised on {} ({})", m_RendererName, m_VersionName);
}

void D3D12Renderer::Destroy()
{
	if (m_Initialised == false)
	{
		return;
	}

	// Never destroy anything the GPU might still be using.
	WaitForGpu();

	ImGui_ImplDX12_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	m_ShaderLibrary.reset();

	for (auto& list : m_DeferredReleases)
	{
		list.clear();
	}

	m_Initialised = false;

	m_RootSignature.Reset();
	m_ShaderHeap.Destroy();
	m_StagingHeap.Destroy();
	m_RTVHeap.Destroy();

	for (auto& buffer : m_BackBuffers)
	{
		buffer.Reset();
	}

	for (auto& allocator : m_CommandAllocators)
	{
		allocator.Reset();
	}

	m_CommandList.Reset();
	m_UploadCommandList.Reset();
	m_UploadAllocator.Reset();
	m_Fence.Reset();

	if (m_FenceEvent)
	{
		CloseHandle(m_FenceEvent);
		m_FenceEvent = nullptr;
	}

	m_SwapChain.Reset();
	m_CommandQueue.Reset();
	m_Device.Reset();
	m_Adapter.Reset();
	m_Factory.Reset();

	s_Instance = nullptr;
	NE_ENGINE_INFO("D3D12 shut down");
}
//----- LIFETIME -----


//----- INITIALISATION -----
void D3D12Renderer::EnableDebugLayer()
{
#ifdef _DEBUG
	// The debug layer validates every API call and prints mistakes to the Output window.
	ComPtr<ID3D12Debug> debug;

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
	{
		debug->EnableDebugLayer();
		NE_ENGINE_INFO("D3D12 debug layer enabled");
	}
#endif
}

void D3D12Renderer::CreateDevice()
{
	UINT factoryFlags = 0;
#ifdef _DEBUG
	factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	// The debug factory needs Windows' optional "Graphics Tools" feature. If it isn't installed, fall back to a normal factory instead of failing.
	if (FAILED(CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&m_Factory))))
	{
		NE_ENGINE_WARN("Couldn't create a debug DXGI factory (install 'Graphics Tools' in Windows Optional Features for D3D12 debugging)");
		NE_D3D_CHECK(CreateDXGIFactory2(0, IID_PPV_ARGS(&m_Factory)));
	}

	// ---- Pick a GPU ----
	// Prefer the high-performance GPU (matters on laptops with integrated + dedicated GPUs).
	ComPtr<IDXGIFactory6> factory6;

	if (SUCCEEDED(m_Factory.As(&factory6)))
	{
		for (UINT i = 0; factory6->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&m_Adapter)) != DXGI_ERROR_NOT_FOUND; i++)
		{
			DXGI_ADAPTER_DESC1 desc;
			m_Adapter->GetDesc1(&desc);

			// Skip the software rasteriser ("Microsoft Basic Render Driver").
			if ((desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 && SUCCEEDED(D3D12CreateDevice(m_Adapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)))
			{
				break;
			}

			m_Adapter.Reset();
		}
	}

	// Older Windows: just take the first adapter that supports D3D12.
	if (m_Adapter == nullptr)
	{
		for (UINT i = 0; m_Factory->EnumAdapters1(i, &m_Adapter) != DXGI_ERROR_NOT_FOUND; i++)
		{
			if (SUCCEEDED(D3D12CreateDevice(m_Adapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)))
			{
				break;
			}

			m_Adapter.Reset();
		}
	}

	NE_ENGINE_ASSERT(m_Adapter, "No GPU with Direct3D 12 support found");

	// Feature level 11_0 is the minimum D3D12 runs on, and plenty for 2D.
	NE_D3D_CHECK(D3D12CreateDevice(m_Adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device)));
	m_Device->SetName(L"NoctalEngine Device");

#ifdef _DEBUG
	// Make the debugger stop on real errors, instead of just printing them.
	ComPtr<ID3D12InfoQueue> infoQueue;

	if (SUCCEEDED(m_Device.As(&infoQueue)))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
	}
#endif

	// ---- Info strings for the ImGui "Renderer" window ----
	DXGI_ADAPTER_DESC1 adapterDesc;
	m_Adapter->GetDesc1(&adapterDesc);

	m_RendererName = D3D12Helpers::ToUTF8(adapterDesc.Description);

	switch (adapterDesc.VendorId)
	{
		case 0x10DE: m_VendorName = "NVIDIA"; break;
		case 0x1002: m_VendorName = "AMD"; break;
		case 0x8086: m_VendorName = "Intel"; break;
		default:     m_VendorName = "Unknown vendor"; break;
	}

	const D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_12_0, D3D_FEATURE_LEVEL_12_1 };
	D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevels = {};
	featureLevels.NumFeatureLevels = _countof(levels);
	featureLevels.pFeatureLevelsRequested = levels;

	if (SUCCEEDED(m_Device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevels, sizeof(featureLevels))))
	{
		const uint32_t level = (uint32_t)featureLevels.MaxSupportedFeatureLevel;
		m_VersionName = "Direct3D 12 (Feature Level " + std::to_string(level >> 12) + "_" + std::to_string((level >> 8) & 0xF) + ")";
	}

	// ---- Command queue ----
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	NE_D3D_CHECK(m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue)));
	m_CommandQueue->SetName(L"Main Command Queue");
}

void D3D12Renderer::CreateCommandObjects()
{
	for (uint32_t i = 0; i < D3D12Config::FramesInFlight; i++)
	{
		NE_D3D_CHECK(m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocators[i])));
	}

	NE_D3D_CHECK(m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocators[0].Get(), nullptr, IID_PPV_ARGS(&m_CommandList)));
	m_CommandList->SetName(L"Main Command List");
	NE_D3D_CHECK(m_CommandList->Close());

	NE_D3D_CHECK(m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_UploadAllocator)));
	NE_D3D_CHECK(m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_UploadAllocator.Get(), nullptr, IID_PPV_ARGS(&m_UploadCommandList)));
	m_UploadCommandList->SetName(L"Upload Command List");
	NE_D3D_CHECK(m_UploadCommandList->Close());

	NE_D3D_CHECK(m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));
	m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	NE_ENGINE_ASSERT(m_FenceEvent, "Failed to create fence event");
}

void D3D12Renderer::CreateSwapChain(HWND hwnd)
{
	ComPtr<IDXGIFactory5> factory5;

	if (SUCCEEDED(m_Factory.As(&factory5)))
	{
		BOOL allowTearing = FALSE;

		if (SUCCEEDED(factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing))))
		{
			m_TearingSupported = allowTearing == TRUE;
		}
	}

	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.Width = m_Width;
	desc.Height = m_Height;
	desc.Format = D3D12Config::BackBufferFormat;
	desc.SampleDesc = { 1, 0 };
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = D3D12Config::FramesInFlight;
	desc.Scaling = DXGI_SCALING_STRETCH;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	
	desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	desc.Flags = m_TearingSupported ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

	ComPtr<IDXGISwapChain1> swapChain1;
	NE_D3D_CHECK(m_Factory->CreateSwapChainForHwnd(m_CommandQueue.Get(), hwnd, &desc, nullptr, nullptr, &swapChain1));
	NE_D3D_CHECK(swapChain1.As(&m_SwapChain));

	m_Factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);

	m_FrameIndex = m_SwapChain->GetCurrentBackBufferIndex();
}

void D3D12Renderer::CreateDescriptorHeaps()
{
	// RTVs: one per back buffer.
	m_RTVHeap.Create(m_Device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12Config::FramesInFlight, false, L"RTV Heap");

	// Staging SRVs (CPU only).
	m_StagingHeap.Create(m_Device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12Config::StagingDescriptorCount, false, L"Staging SRV Heap");
	m_StagingAllocator.Init(0, D3D12Config::StagingDescriptorCount);

	// A "null" SRV: a valid descriptor pointing at nothing. Unused texture slots point at this, so every slot in a texture table is always valid.
	m_NullSRVIndex = m_StagingAllocator.Allocate();

	D3D12_SHADER_RESOURCE_VIEW_DESC nullDesc = {};
	nullDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	nullDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	nullDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	nullDesc.Texture2D.MipLevels = 1;
	m_Device->CreateShaderResourceView(nullptr, &nullDesc, m_StagingHeap.GetCPUHandle(m_NullSRVIndex));

	m_PendingTextureSlots.fill(m_StagingHeap.GetCPUHandle(m_NullSRVIndex));

	// Shader-visible heap: ImGui block + one block per frame in flight.
	const uint32_t shaderHeapSize = D3D12Config::ImGuiDescriptorCount + D3D12Config::FramesInFlight * D3D12Config::PerFrameDescriptorCount;
	m_ShaderHeap.Create(m_Device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, shaderHeapSize, true, L"Shader Visible Heap");
	m_ImGuiDescriptors.Init(0, D3D12Config::ImGuiDescriptorCount);
}

void D3D12Renderer::CreateRenderTargets()
{
	for (uint32_t i = 0; i < D3D12Config::FramesInFlight; i++)
	{
		NE_D3D_CHECK(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_BackBuffers[i])));
		m_Device->CreateRenderTargetView(m_BackBuffers[i].Get(), nullptr, m_RTVHeap.GetCPUHandle(i));

		const std::wstring name = L"Back Buffer " + std::to_wstring(i);
		m_BackBuffers[i]->SetName(name.c_str());
	}
}

void D3D12Renderer::CreateRootSignature()
{
	// Parameter 0: the camera constant buffer (b0), used by the vertex shader.
	// Parameter 1: a table of MaxTextureSlots textures (t0..t31), used by the pixel shader.
	// Plus one "static" sampler (s0) baked into the signature, so it never needs binding.

	D3D12_DESCRIPTOR_RANGE textureRange = {};
	textureRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	textureRange.NumDescriptors = NoctalEngine::RendererConfig::MaxTextureSlots;
	textureRange.BaseShaderRegister = 0;
	textureRange.RegisterSpace = 0;
	textureRange.OffsetInDescriptorsFromTableStart = 0;

	D3D12_ROOT_PARAMETER parameters[2] = {};

	parameters[RootParam_CameraCBV].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	parameters[RootParam_CameraCBV].Descriptor.ShaderRegister = 0;
	parameters[RootParam_CameraCBV].Descriptor.RegisterSpace = 0;
	parameters[RootParam_CameraCBV].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	parameters[RootParam_Textures].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	parameters[RootParam_Textures].DescriptorTable.NumDescriptorRanges = 1;
	parameters[RootParam_Textures].DescriptorTable.pDescriptorRanges = &textureRange;
	parameters[RootParam_Textures].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_STATIC_SAMPLER_DESC sampler = {};
	sampler.Filter = D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.MipLODBias = 0.0f;
	sampler.MaxAnisotropy = 1;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.MinLOD = 0.0f;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = 0;
	sampler.RegisterSpace = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_DESC desc = {};
	desc.NumParameters = _countof(parameters);
	desc.pParameters = parameters;
	desc.NumStaticSamplers = 1;
	desc.pStaticSamplers = &sampler;
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
				 D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
				 D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
				 D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

	// The description is "serialised" into a blob, then the blob becomes the root signature.
	ComPtr<ID3DBlob> blob;
	ComPtr<ID3DBlob> error;
	const HRESULT hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error);

	if (FAILED(hr))
	{
		NE_ENGINE_ERROR("Root signature serialisation failed: {}", error ? (const char*)error->GetBufferPointer() : "unknown error");
		NE_ENGINE_ASSERT(false, "Root signature creation failed");
		return;
	}

	NE_D3D_CHECK(m_Device->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)));
	m_RootSignature->SetName(L"Batch2D Root Signature");
}

void D3D12Renderer::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags  |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags  |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags  |= ImGuiConfigFlags_ViewportsEnable;
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	ImGui::StyleColorsDark();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplSDL3_InitForD3D(m_Window);

	ImGui_ImplDX12_InitInfo info;
	info.Device = m_Device.Get();
	info.CommandQueue = m_CommandQueue.Get();
	info.NumFramesInFlight = D3D12Config::FramesInFlight;
	info.RTVFormat = D3D12Config::BackBufferFormat;
	info.DSVFormat = DXGI_FORMAT_UNKNOWN;
	info.SrvDescriptorHeap = m_ShaderHeap.GetHeap();

	info.SrvDescriptorAllocFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE* outCPU, D3D12_GPU_DESCRIPTOR_HANDLE* outGPU)
	{
		D3D12Renderer& renderer = D3D12Renderer::Get();
		const uint32_t index = renderer.m_ImGuiDescriptors.Allocate();
		*outCPU = renderer.m_ShaderHeap.GetCPUHandle(index);
		*outGPU = renderer.m_ShaderHeap.GetGPUHandle(index);
	};

	info.SrvDescriptorFreeFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE cpu, D3D12_GPU_DESCRIPTOR_HANDLE)
	{
		D3D12Renderer& renderer = D3D12Renderer::Get();
		renderer.m_ImGuiDescriptors.Free(renderer.m_ShaderHeap.GetIndex(cpu));
	};

	ImGui_ImplDX12_Init(&info);
}
//----- INITIALISATION -----

//----- FRAME -----
void D3D12Renderer::BeginFrame()
{
	NE_SCOPE_TIMER("D3D12Renderer", "BeginFrame");

	// ---- ImGui ----
	ImGuiIO& io = ImGui::GetIO();
	NoctalEngine::Application& app = NoctalEngine::Application::Get();
	io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	// ---- Start recording ----
	// Safe to reset: MoveToNextFrame() already waited for the GPU to finish with this frame slot.
	NE_D3D_CHECK(m_CommandAllocators[m_FrameIndex]->Reset());
	NE_D3D_CHECK(m_CommandList->Reset(m_CommandAllocators[m_FrameIndex].Get(), nullptr));

	// New frame = fresh block of texture-table descriptors, and no textures bound yet.
	m_FrameDescriptorOffset = 0;
	m_PendingTextureSlots.fill(m_StagingHeap.GetCPUHandle(m_NullSRVIndex));
	m_TextureSlotsDirty = true;

	//Assign back buffer as render target
	const D3D12_RESOURCE_BARRIER toRenderTarget = D3D12Helpers::TransitionBarrier(m_BackBuffers[m_FrameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_CommandList->ResourceBarrier(1, &toRenderTarget);

	const D3D12_CPU_DESCRIPTOR_HANDLE rtv = m_RTVHeap.GetCPUHandle(m_FrameIndex);

	const float clearColour[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
	m_CommandList->ClearRenderTargetView(rtv, clearColour, 0, nullptr);
	m_CommandList->OMSetRenderTargets(1, &rtv, FALSE, nullptr);

	// Draw to the whole window.
	D3D12_VIEWPORT viewport = { 0.0f, 0.0f, (float)m_Width, (float)m_Height, 0.0f, 1.0f };
	D3D12_RECT scissor = { 0, 0, (LONG)m_Width, (LONG)m_Height };
	m_CommandList->RSSetViewports(1, &viewport);
	m_CommandList->RSSetScissorRects(1, &scissor);

	ID3D12DescriptorHeap* heaps[] = { m_ShaderHeap.GetHeap() };
	m_CommandList->SetDescriptorHeaps(1, heaps);
	m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
}

void D3D12Renderer::EndFrame()
{
	NE_SCOPE_TIMER("D3D12Renderer", "EndFrame");

	{
		NE_SCOPE_TIMER("D3D12Renderer", "ImGui");
		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), m_CommandList.Get());
	}

	// Make back buffer presentable, drawing is finished
	const D3D12_RESOURCE_BARRIER toPresent = D3D12Helpers::TransitionBarrier(m_BackBuffers[m_FrameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_CommandList->ResourceBarrier(1, &toPresent);

	// ---- Submit ----
	NE_D3D_CHECK(m_CommandList->Close());
	ID3D12CommandList* lists[] = { m_CommandList.Get() };
	m_CommandQueue->ExecuteCommandLists(1, lists);

	// ImGui windows dragged outside the main window (multi-viewport) render themselves.
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	// ---- Present ----
	{
		NE_SCOPE_TIMER("D3D12Renderer", "Present");

		// Interval 1 = wait for the monitor's refresh. 0 + ALLOW_TEARING = present immediately.
		const bool vsync = NoctalEngine::Application::Get().GetWindow().IsVSync();
		const UINT syncInterval = vsync ? 1 : 0;
		const UINT presentFlags = (vsync == false && m_TearingSupported) ? DXGI_PRESENT_ALLOW_TEARING : 0;

		const HRESULT hr = m_SwapChain->Present(syncInterval, presentFlags);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			NE_ENGINE_ERROR("GPU device removed (reason 0x{:08X})", (uint32_t)m_Device->GetDeviceRemovedReason());
			NE_ENGINE_ASSERT(false, "D3D12 device lost");
		}
	}

	{
		NE_SCOPE_TIMER("D3D12Renderer", "Wait for GPU frame");
		MoveToNextFrame();
	}
}

void D3D12Renderer::OnWindowResize(const uint32_t width, const uint32_t height)
{
	if (m_SwapChain == nullptr || width == 0 || height == 0)
	{
		return;
	}

	if (width == m_Width && height == m_Height)
	{
		return;
	}

	// The back buffers can't be resized while the GPU might be using them.
	WaitForGpu();

	for (uint32_t i = 0; i < D3D12Config::FramesInFlight; i++)
	{
		m_BackBuffers[i].Reset();
		m_FrameFenceValues[i] = m_FenceValue;
	}

	DXGI_SWAP_CHAIN_DESC desc = {};
	m_SwapChain->GetDesc(&desc);
	NE_D3D_CHECK(m_SwapChain->ResizeBuffers(D3D12Config::FramesInFlight, width, height, desc.BufferDesc.Format, desc.Flags));

	m_Width = width;
	m_Height = height;
	m_FrameIndex = m_SwapChain->GetCurrentBackBufferIndex();

	CreateRenderTargets();
}
//----- FRAME -----

//----- SYNCHRONISATION -----
void D3D12Renderer::WaitForFenceValue(uint64_t fenceValue)
{
	if (m_Fence->GetCompletedValue() < fenceValue)
	{
		NE_D3D_CHECK(m_Fence->SetEventOnCompletion(fenceValue, m_FenceEvent));
		WaitForSingleObject(m_FenceEvent, INFINITE);
	}
}

void D3D12Renderer::WaitForGpu()
{
	const uint64_t value = ++m_FenceValue;
	NE_D3D_CHECK(m_CommandQueue->Signal(m_Fence.Get(), value));
	WaitForFenceValue(value);
}

void D3D12Renderer::MoveToNextFrame()
{
	const uint64_t value = ++m_FenceValue;
	NE_D3D_CHECK(m_CommandQueue->Signal(m_Fence.Get(), value));
	m_FrameFenceValues[m_FrameIndex] = value;

	m_FrameIndex = m_SwapChain->GetCurrentBackBufferIndex();

	// If the GPU is still working on the frame that last used this slot, wait for it.
	WaitForFenceValue(m_FrameFenceValues[m_FrameIndex]);

	// That frame is done on the GPU, so anything released during it can finally go.
	m_DeferredReleases[m_FrameIndex].clear();

	m_FrameNumber++;
}
//----- SYNCHRONISATION -----

//----- RESOURCE HELPERS -----
ComPtr<ID3D12Resource> D3D12Renderer::CreateBuffer(uint64_t size, D3D12_HEAP_TYPE heapType, const wchar_t* debugName)
{
	const D3D12_HEAP_PROPERTIES heap = D3D12Helpers::HeapProperties(heapType);
	const D3D12_RESOURCE_DESC desc = D3D12Helpers::BufferDesc(size);

	// Upload heap resources must be in GENERIC_READ.
	const D3D12_RESOURCE_STATES state = heapType == D3D12_HEAP_TYPE_UPLOAD ? D3D12_RESOURCE_STATE_GENERIC_READ : D3D12_RESOURCE_STATE_COMMON;

	ComPtr<ID3D12Resource> buffer;
	NE_D3D_CHECK(m_Device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc, state, nullptr, IID_PPV_ARGS(&buffer)));

	if (buffer && debugName)
	{
		buffer->SetName(debugName);
	}

	return buffer;
}

void D3D12Renderer::ExecuteImmediate(const std::function<void(ID3D12GraphicsCommandList*)>& recordCommands)
{
	NE_D3D_CHECK(m_UploadAllocator->Reset());
	NE_D3D_CHECK(m_UploadCommandList->Reset(m_UploadAllocator.Get(), nullptr));

	recordCommands(m_UploadCommandList.Get());

	NE_D3D_CHECK(m_UploadCommandList->Close());
	ID3D12CommandList* lists[] = { m_UploadCommandList.Get() };
	m_CommandQueue->ExecuteCommandLists(1, lists);

	WaitForGpu();
}

void D3D12Renderer::DeferRelease(ComPtr<IUnknown> object)
{
	if (object == nullptr)
	{
		return;
	}

	m_DeferredReleases[m_FrameIndex].push_back(std::move(object));
}

void D3D12Renderer::SetTextureSlot(uint32_t slot, D3D12_CPU_DESCRIPTOR_HANDLE srv)
{
	NE_ENGINE_ASSERT(slot < NoctalEngine::RendererConfig::MaxTextureSlots, "Texture slot out of range");

	// Only mark dirty if it actually changed.
	if (m_PendingTextureSlots[slot].ptr != srv.ptr)
	{
		m_PendingTextureSlots[slot] = srv;
		m_TextureSlotsDirty = true;
	}
}
//----- RESOURCE HELPERS -----

//----- DRAWING -----
void D3D12Renderer::DrawIndexed(uint32_t indexCount)
{
	if (m_TextureSlotsDirty == true)
	{
		constexpr uint32_t slotCount = NoctalEngine::RendererConfig::MaxTextureSlots;

		if (m_FrameDescriptorOffset + slotCount > D3D12Config::PerFrameDescriptorCount)
		{
			NE_ENGINE_ERROR("Out of per-frame descriptors. Raise D3D12Config::PerFrameDescriptorCount");
			return;
		}

		// Grab the next free block of 'slotCount' descriptors in this frame's section of the heap.
		const uint32_t firstIndex = D3D12Config::ImGuiDescriptorCount + m_FrameIndex * D3D12Config::PerFrameDescriptorCount + m_FrameDescriptorOffset;
		m_FrameDescriptorOffset += slotCount;

		// Copy the pending textures' SRVs into that block, so the shader sees them as one contiguous array: g_Textures[0..31].
		const D3D12_CPU_DESCRIPTOR_HANDLE destination = m_ShaderHeap.GetCPUHandle(firstIndex);
		const UINT destinationSize = slotCount;

		// nullptr source sizes = each source range is 1 descriptor.
		m_Device->CopyDescriptors(1, &destination, &destinationSize, slotCount, m_PendingTextureSlots.data(), nullptr, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		m_CommandList->SetGraphicsRootDescriptorTable(RootParam_Textures, m_ShaderHeap.GetGPUHandle(firstIndex));
		m_TextureSlotsDirty = false;
	}

	m_CommandList->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
}
//----- DRAWING -----

//----- RESOURCE FACTORY -----

std::shared_ptr<NoctalEngine::Shader> D3D12Renderer::CreateShader(const std::string& filePath)
{
	return std::make_shared<D3D12Shader>(filePath);
}

std::shared_ptr<NoctalEngine::Texture> D3D12Renderer::CreateTexture(const std::string& path)
{
	return std::make_shared<D3D12Texture2D>(path);
}

std::shared_ptr<NoctalEngine::Texture> D3D12Renderer::CreateTexture(uint32_t width, uint32_t height, const void* rgbaPixels)
{
	return std::make_shared<D3D12Texture2D>(width, height, rgbaPixels);
}

std::unique_ptr<NoctalEngine::VertexBuffer> D3D12Renderer::CreateVertexBuffer(uint32_t maxBytesPerFrame, const NoctalEngine::BufferLayout& layout)
{
	return std::make_unique<D3D12VertexBuffer>(maxBytesPerFrame, layout);
}

std::unique_ptr<NoctalEngine::IndexBuffer> D3D12Renderer::CreateIndexBuffer(const void* indices, uint32_t count, NoctalEngine::IndexFormat format)
{
	return std::make_unique<D3D12IndexBuffer>(indices, count, format);
}

std::unique_ptr<NoctalEngine::ConstantBuffer> D3D12Renderer::CreateConstantBuffer(uint32_t size, uint32_t bindSlot)
{
	return std::make_unique<D3D12ConstantBuffer>(size, bindSlot);
}

std::unique_ptr<NoctalEngine::Pipeline> D3D12Renderer::CreatePipeline(const NoctalEngine::PipelineSpecification& specification)
{
	return std::make_unique<D3D12Pipeline>(specification);
}
//----- RESOURCE FACTORY -----