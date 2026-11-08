#pragma once
#include "NoctalEngine/Rendering/Buffers/VertexBuffer.h"
#include "D3D12FrameLinearAllocator.h"

// Dynamic vertex buffer. Every SetData() gets fresh memory from the upload ring, so batches
// recorded earlier in the frame (still waiting to be drawn by the GPU) are never overwritten.
class D3D12VertexBuffer : public NoctalEngine::VertexBuffer
{
public:
	D3D12VertexBuffer(uint32_t maxBytesPerFrame, const NoctalEngine::BufferLayout& layout);
	virtual ~D3D12VertexBuffer() override = default;

	virtual void Bind() override;
	virtual void SetData(const void* data, uint32_t size) override;

	virtual const NoctalEngine::BufferLayout& GetLayout() const override { return m_Layout; }

private:
	NoctalEngine::BufferLayout m_Layout;
	D3D12FrameLinearAllocator m_Ring;

	// A "view" tells the input assembler where the vertices are, how big, and the stride.
	D3D12_VERTEX_BUFFER_VIEW m_View = {};
};
