#include "nepch.h"
#include "D3D12IndexBuffer.h"
#include "NoctalEngine/Rendering/D3D12/D3D12Renderer.h"

D3D12IndexBuffer::D3D12IndexBuffer(const void* indices, uint32_t count, NoctalEngine::IndexFormat format) : m_Count(count), m_Format(format)
{
	D3D12Renderer& renderer = D3D12Renderer::Get();

	const uint32_t indexSize = format == NoctalEngine::IndexFormat::UINT16 ? sizeof(uint16_t) : sizeof(uint32_t);
	const uint32_t sizeInBytes = indexSize * count;

	// The GPU-only buffer we'll keep...
	m_Buffer = renderer.CreateBuffer(sizeInBytes, D3D12_HEAP_TYPE_DEFAULT, L"Index Buffer");

	// ...and a temporary CPU-visible buffer to get the data there.
	ComPtr<ID3D12Resource> upload = renderer.CreateBuffer(sizeInBytes, D3D12_HEAP_TYPE_UPLOAD, L"Index Buffer Upload");

	void* mapped = nullptr;
	D3D12_RANGE readRange = { 0, 0 };
	NE_D3D_CHECK(upload->Map(0, &readRange, &mapped));
	memcpy(mapped, indices, sizeInBytes);
	upload->Unmap(0, nullptr);

	// Copy upload -> default on the GPU, then mark it as an index buffer.
	renderer.ExecuteImmediate([&](ID3D12GraphicsCommandList* commandList)
	{
		commandList->CopyBufferRegion(m_Buffer.Get(), 0, upload.Get(), 0, sizeInBytes);

		// Buffers start in COMMON and are automatically "promoted" to COPY_DEST by the copy.
		const D3D12_RESOURCE_BARRIER barrier = D3D12Helpers::TransitionBarrier(m_Buffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);
		commandList->ResourceBarrier(1, &barrier);
	});

	// ExecuteImmediate waited for the GPU, so 'upload' can safely be released when it goes out of scope.

	m_View.BufferLocation = m_Buffer->GetGPUVirtualAddress();
	m_View.SizeInBytes = sizeInBytes;
	m_View.Format = format == NoctalEngine::IndexFormat::UINT16 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
}

D3D12IndexBuffer::~D3D12IndexBuffer()
{
	if (D3D12Renderer::IsActive())
	{
		D3D12Renderer::Get().DeferRelease(m_Buffer);
	}
}

void D3D12IndexBuffer::Bind()
{
	D3D12Renderer::Get().GetCommandList()->IASetIndexBuffer(&m_View);
}
