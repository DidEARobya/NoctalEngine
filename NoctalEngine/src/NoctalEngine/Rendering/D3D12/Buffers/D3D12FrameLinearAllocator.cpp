#include "nepch.h"
#include "D3D12FrameLinearAllocator.h"
#include "NoctalEngine/Rendering/D3D12/D3D12Renderer.h"

D3D12FrameLinearAllocator::~D3D12FrameLinearAllocator()
{
	Destroy();
}

void D3D12FrameLinearAllocator::Create(uint64_t bytesPerFrame, const wchar_t* debugName)
{
	D3D12Renderer& renderer = D3D12Renderer::Get();

	// Keep each section 256-byte aligned (constant buffers need this).
	m_BytesPerFrame = D3D12Helpers::AlignUp(bytesPerFrame, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
	m_Buffer = renderer.CreateBuffer(m_BytesPerFrame * D3D12Config::FramesInFlight, D3D12_HEAP_TYPE_UPLOAD, debugName);

	// Map once and keep it mapped. Upload heap memory is fine to leave mapped forever.
	// The empty read range tells D3D we won't read from it on the CPU.
	D3D12_RANGE readRange = { 0, 0 };
	NE_D3D_CHECK(m_Buffer->Map(0, &readRange, (void**)&m_MappedData));

	m_GPUAddress = m_Buffer->GetGPUVirtualAddress();
}

void D3D12FrameLinearAllocator::Destroy()
{
	if (m_Buffer == nullptr)
	{
		return;
	}

	m_Buffer->Unmap(0, nullptr);
	m_MappedData = nullptr;

	// The GPU might still be reading the last frame's data, so let the renderer release it later.
	if (D3D12Renderer::IsActive())
	{
		D3D12Renderer::Get().DeferRelease(m_Buffer);
	}

	m_Buffer.Reset();
}

D3D12FrameLinearAllocator::Allocation D3D12FrameLinearAllocator::Allocate(uint64_t size, uint64_t alignment)
{
	const D3D12Renderer& renderer = D3D12Renderer::Get();

	// First allocation of a new frame: start this frame's section from the beginning.
	if (renderer.GetFrameNumber() != m_LastFrameNumber)
	{
		m_LastFrameNumber = renderer.GetFrameNumber();
		m_Offset = 0;
	}

	const uint64_t alignedOffset = D3D12Helpers::AlignUp(m_Offset, alignment);

	if (alignedOffset + size > m_BytesPerFrame)
	{
		NE_ENGINE_ERROR("D3D12FrameLinearAllocator is full for this frame ({} bytes requested)", size);
		return Allocation();
	}

	m_Offset = alignedOffset + size;

	const uint64_t bufferOffset = renderer.GetFrameIndex() * m_BytesPerFrame + alignedOffset;

	Allocation allocation;
	allocation.CPU = m_MappedData + bufferOffset;
	allocation.GPU = m_GPUAddress + bufferOffset;
	return allocation;
}
