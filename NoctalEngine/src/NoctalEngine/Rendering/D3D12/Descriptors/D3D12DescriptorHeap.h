#pragma once
#include "NoctalEngine/Rendering/D3D12/D3D12Common.h"

// ---------------------------------------------------------------------------------
// Descriptors, quickly:
// A descriptor is a small struct that describes a resource to the GPU ("this is a 256x256
// RGBA texture at address X"). They live in descriptor heaps (arrays of descriptors).
//
// - Shader-visible heaps are what shaders actually read from, but there's only one bound at
//   a time and writing to them from the CPU is slow-ish.
// - Non-shader-visible ("staging") heaps are plain CPU memory. We create descriptors here
//   once, then copy them into the shader-visible heap when a draw needs them.
// ---------------------------------------------------------------------------------

class D3D12DescriptorHeap
{
public:
	void Create(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t capacity, bool shaderVisible, const wchar_t* debugName);
	void Destroy();

	ID3D12DescriptorHeap* GetHeap() const { return m_Heap.Get(); }
	uint32_t GetCapacity() const { return m_Capacity; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint32_t index) const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32_t index) const;

	// Turns a CPU handle back into its index in this heap.
	uint32_t GetIndex(D3D12_CPU_DESCRIPTOR_HANDLE handle) const;

private:
	ComPtr<ID3D12DescriptorHeap> m_Heap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_CPUStart = {};
	D3D12_GPU_DESCRIPTOR_HANDLE m_GPUStart = {};
	uint32_t m_DescriptorSize = 0;	// Bytes between descriptors. Differs per GPU, so we must ask.
	uint32_t m_Capacity = 0;
};

// Hands out single descriptor indices from a range of a heap, and takes them back.
// Used for long-lived descriptors (one per texture, ImGui's font, ...).
class D3D12DescriptorAllocator
{
public:
	static constexpr uint32_t InvalidIndex = UINT32_MAX;

	void Init(uint32_t firstIndex, uint32_t count);

	uint32_t Allocate();
	void Free(uint32_t index);

private:
	// A "free list": just a stack of indices that are available.
	std::vector<uint32_t> m_FreeIndices;
};
