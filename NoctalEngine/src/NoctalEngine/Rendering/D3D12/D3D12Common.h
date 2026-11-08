#pragma once

// ---------------------------------------------------------------------------------
// D3D12Common
// Shared includes and small helpers for every D3D12 file.
// Only D3D12 .cpp/.h files include this, so the rest of the engine never sees <windows.h>.
// ---------------------------------------------------------------------------------

#ifndef NOMINMAX
	#define NOMINMAX				// stops windows.h defining min/max macros (they break std::min/max)
#endif
#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN		// skips rarely used parts of windows.h
#endif

#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

// ComPtr is a smart pointer for COM objects (all D3D objects are COM objects).
// It calls Release() for us, like a shared_ptr that uses the object's own ref count.
template<typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

// Checks an HRESULT (D3D's error code) and logs + breaks if it failed.
#define NE_D3D_CHECK(call) { HRESULT hr__ = (call); if (FAILED(hr__)) { NE_ENGINE_ERROR("D3D12 call failed: {} (HRESULT 0x{:08X})", #call, (uint32_t)hr__); DEBUG_BREAK(); } }

namespace D3D12Config
{
	// How many frames the CPU can record ahead of the GPU. Also the number of swap chain buffers.
	// 2 = while the GPU draws frame N, the CPU records frame N+1.
	constexpr uint32_t FramesInFlight = 2;

	constexpr DXGI_FORMAT BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Shader-visible descriptor space (see D3D12Renderer for how it's split up).
	constexpr uint32_t ImGuiDescriptorCount = 64;
	constexpr uint32_t PerFrameDescriptorCount = 4096;

	// Non-shader-visible "staging" descriptors. Every texture keeps its SRV here.
	constexpr uint32_t StagingDescriptorCount = 4096;
}

namespace D3D12Helpers
{
	// Rounds 'value' up to the next multiple of 'alignment' (alignment must be a power of 2).
	inline uint64_t AlignUp(uint64_t value, uint64_t alignment)
	{
		return (value + alignment - 1) & ~(alignment - 1);
	}

	// A barrier tells the GPU "this resource is changing how it's used" (e.g. from being
	// presented to being drawn into), so it can flush caches / wait as needed.
	inline D3D12_RESOURCE_BARRIER TransitionBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
	{
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = resource;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = before;
		barrier.Transition.StateAfter = after;
		return barrier;
	}

	// DEFAULT heap = GPU-only memory (fastest for the GPU).
	// UPLOAD heap  = CPU-writable memory the GPU can read (used to send data over).
	inline D3D12_HEAP_PROPERTIES HeapProperties(D3D12_HEAP_TYPE type)
	{
		D3D12_HEAP_PROPERTIES properties = {};
		properties.Type = type;
		properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		properties.CreationNodeMask = 1;
		properties.VisibleNodeMask = 1;
		return properties;
	}

	inline D3D12_RESOURCE_DESC BufferDesc(uint64_t size)
	{
		D3D12_RESOURCE_DESC desc = {};
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Alignment = 0;
		desc.Width = size;
		desc.Height = 1;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.SampleDesc = { 1, 0 };
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;
		return desc;
	}

	// Converts a UTF-8 std::string to a wide string (most Windows APIs want wide strings).
	inline std::wstring ToWide(const std::string& text)
	{
		if (text.empty())
		{
			return std::wstring();
		}

		const int length = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), (int)text.size(), nullptr, 0);
		std::wstring result(length, L'\0');
		MultiByteToWideChar(CP_UTF8, 0, text.c_str(), (int)text.size(), result.data(), length);
		return result;
	}

	inline std::string ToUTF8(const std::wstring& text)
	{
		if (text.empty())
		{
			return std::string();
		}

		const int length = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), (int)text.size(), nullptr, 0, nullptr, nullptr);
		std::string result(length, '\0');
		WideCharToMultiByte(CP_UTF8, 0, text.c_str(), (int)text.size(), result.data(), length, nullptr, nullptr);
		return result;
	}
}
