#include "nepch.h"
#include "D3D12DescriptorHeap.h"

void D3D12DescriptorHeap::Create(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t capacity, bool shaderVisible, const wchar_t* debugName)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = type;
	desc.NumDescriptors = capacity;
	desc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask = 0;

	NE_D3D_CHECK(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_Heap)));
	m_Heap->SetName(debugName);

	m_Capacity = capacity;
	m_DescriptorSize = device->GetDescriptorHandleIncrementSize(type);
	m_CPUStart = m_Heap->GetCPUDescriptorHandleForHeapStart();

	// Only shader-visible heaps have GPU addresses.
	if (shaderVisible)
	{
		m_GPUStart = m_Heap->GetGPUDescriptorHandleForHeapStart();
	}
}

void D3D12DescriptorHeap::Destroy()
{
	m_Heap.Reset();
	m_Capacity = 0;
}

D3D12_CPU_DESCRIPTOR_HANDLE D3D12DescriptorHeap::GetCPUHandle(uint32_t index) const
{
	NE_ENGINE_ASSERT(index < m_Capacity, "Descriptor index out of range");

	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_CPUStart;
	handle.ptr += (SIZE_T)index * m_DescriptorSize;
	return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE D3D12DescriptorHeap::GetGPUHandle(uint32_t index) const
{
	NE_ENGINE_ASSERT(index < m_Capacity, "Descriptor index out of range");

	D3D12_GPU_DESCRIPTOR_HANDLE handle = m_GPUStart;
	handle.ptr += (UINT64)index * m_DescriptorSize;
	return handle;
}

uint32_t D3D12DescriptorHeap::GetIndex(D3D12_CPU_DESCRIPTOR_HANDLE handle) const
{
	return (uint32_t)((handle.ptr - m_CPUStart.ptr) / m_DescriptorSize);
}

void D3D12DescriptorAllocator::Init(uint32_t firstIndex, uint32_t count)
{
	m_FreeIndices.clear();
	m_FreeIndices.reserve(count);

	// Push in reverse so the lowest index is handed out first (purely cosmetic, easier debugging).
	for (uint32_t i = count; i > 0; i--)
	{
		m_FreeIndices.push_back(firstIndex + i - 1);
	}
}

uint32_t D3D12DescriptorAllocator::Allocate()
{
	if (m_FreeIndices.empty())
	{
		NE_ENGINE_ERROR("D3D12DescriptorAllocator is out of descriptors");
		return InvalidIndex;
	}

	const uint32_t index = m_FreeIndices.back();
	m_FreeIndices.pop_back();
	return index;
}

void D3D12DescriptorAllocator::Free(uint32_t index)
{
	if (index != InvalidIndex)
	{
		m_FreeIndices.push_back(index);
	}
}
