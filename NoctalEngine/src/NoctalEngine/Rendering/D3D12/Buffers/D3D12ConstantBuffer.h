#pragma once
#include "NoctalEngine/Rendering/Buffers/ConstantBuffer.h"
#include "D3D12UploadRing.h"

// Constant buffer that can be updated any number of times per frame (up to MaxUpdatesPerFrame).
// Each update gets its own 256-byte aligned slice of the upload ring, which is then bound
// directly as a root CBV (no descriptor needed).
class D3D12ConstantBuffer : public NoctalEngine::ConstantBuffer
{
public:
	static constexpr uint32_t MaxUpdatesPerFrame = 64;

	D3D12ConstantBuffer(uint32_t size, uint32_t bindSlot);
	virtual ~D3D12ConstantBuffer() override = default;

	virtual void Bind() override;
	virtual void SetData(const void* data, uint32_t size) override;

private:
	D3D12UploadRing m_Ring;
	uint32_t m_AlignedSize = 0;
	uint32_t m_RootParameterIndex = 0;
	D3D12_GPU_VIRTUAL_ADDRESS m_CurrentAddress = 0;
};
