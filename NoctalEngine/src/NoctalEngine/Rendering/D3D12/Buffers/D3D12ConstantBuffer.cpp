#include "nepch.h"
#include "D3D12ConstantBuffer.h"
#include "NoctalEngine/Rendering/D3D12/D3D12Renderer.h"

D3D12ConstantBuffer::D3D12ConstantBuffer(uint32_t size, uint32_t bindSlot)
{
	// D3D12 requires constant buffer addresses to be 256-byte aligned.
	m_AlignedSize = (uint32_t)D3D12Helpers::AlignUp(size, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
	m_Ring.Create((uint64_t)m_AlignedSize * MaxUpdatesPerFrame, L"Constant Buffer");

	// The root signature currently only has one CBV parameter (b0).
	// Add more root parameters in D3D12Renderer::CreateRootSignature to support more slots.
	NE_ENGINE_ASSERT(bindSlot == 0, "D3D12ConstantBuffer only supports register b0 for now");
	m_RootParameterIndex = D3D12Renderer::RootParam_CameraCBV;
}

void D3D12ConstantBuffer::SetData(const void* data, uint32_t size)
{
	NE_ENGINE_ASSERT(size <= m_AlignedSize, "Constant buffer data is bigger than the buffer");

	D3D12UploadRing::Allocation allocation = m_Ring.Allocate(m_AlignedSize, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);

	if (allocation.IsValid() == false)
	{
		return;
	}

	memcpy(allocation.CPU, data, size);
	m_CurrentAddress = allocation.GPU;
}

void D3D12ConstantBuffer::Bind()
{
	if (m_CurrentAddress == 0)
	{
		NE_ENGINE_WARN("Binding a constant buffer that has no data yet");
		return;
	}

	D3D12Renderer::Get().GetCommandList()->SetGraphicsRootConstantBufferView(m_RootParameterIndex, m_CurrentAddress);
}
