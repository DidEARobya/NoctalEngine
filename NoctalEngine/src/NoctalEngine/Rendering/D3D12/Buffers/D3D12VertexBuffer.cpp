#include "nepch.h"
#include "D3D12VertexBuffer.h"
#include "NoctalEngine/Rendering/D3D12/D3D12Renderer.h"

D3D12VertexBuffer::D3D12VertexBuffer(uint32_t maxBytesPerFrame, const NoctalEngine::BufferLayout& layout) : m_Layout(layout)
{
	m_Ring.Create(maxBytesPerFrame, L"Dynamic Vertex Buffer");
}

void D3D12VertexBuffer::SetData(const void* data, uint32_t size)
{
	D3D12FrameLinearAllocator::Allocation allocation = m_Ring.Allocate(size, 16);

	if (allocation.IsValid() == false)
	{
		// Bind an empty view so a failed upload draws nothing rather than stale data.
		m_View = {};
		return;
	}

	// Writing to upload memory goes straight to memory the GPU can read. No copy commands needed.
	memcpy(allocation.CPU, data, size);

	m_View.BufferLocation = allocation.GPU;
	m_View.SizeInBytes = size;
	m_View.StrideInBytes = m_Layout.GetStride();
}

void D3D12VertexBuffer::Bind()
{
	D3D12Renderer::Get().GetCommandList()->IASetVertexBuffers(0, 1, &m_View);
}
