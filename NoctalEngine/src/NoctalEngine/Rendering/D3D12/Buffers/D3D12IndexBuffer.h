#pragma once
#include "NoctalEngine/Rendering/Buffers/IndexBuffer.h"
#include "NoctalEngine/Rendering/D3D12/D3D12Common.h"

// Static index buffer, stored in GPU-only (DEFAULT heap) memory for the fastest reads.
// The data is uploaded once in the constructor.
class D3D12IndexBuffer : public NoctalEngine::IndexBuffer
{
public:
	D3D12IndexBuffer(const void* indices, uint32_t count, NoctalEngine::IndexFormat format);
	virtual ~D3D12IndexBuffer() override;

	virtual void Bind() override;

	virtual uint32_t GetCount() const override { return m_Count; }
	virtual NoctalEngine::IndexFormat GetFormat() const override { return m_Format; }

private:
	ComPtr<ID3D12Resource> m_Buffer;
	D3D12_INDEX_BUFFER_VIEW m_View = {};
	uint32_t m_Count = 0;
	NoctalEngine::IndexFormat m_Format;
};
