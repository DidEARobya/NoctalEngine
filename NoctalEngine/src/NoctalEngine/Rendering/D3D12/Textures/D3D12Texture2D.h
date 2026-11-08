#pragma once
#include "NoctalEngine/Rendering/Textures/Texture2D.h"
#include "NoctalEngine/Rendering/D3D12/D3D12Common.h"

// A 2D RGBA8 texture in GPU memory, plus its Shader Resource View (SRV) descriptor.
// The SRV lives in the renderer's staging heap. Bind() doesn't touch the GPU directly:
// it tells the renderer "put my SRV in slot N", and the renderer copies all the slots
// into a shader-visible table right before the next draw.
class D3D12Texture2D : public NoctalEngine::Texture2D
{
public:
	D3D12Texture2D(const std::string& path);
	D3D12Texture2D(uint32_t width, uint32_t height, const void* rgbaPixels);
	virtual ~D3D12Texture2D() override;

	virtual void Bind() override;
	virtual void SetTextureIndex(int index) override { m_Index = index; }

	virtual uint32_t GetWidth() const override { return m_Width; }
	virtual uint32_t GetHeight() const override { return m_Height; }
	virtual const std::string& GetPath() const override { return m_Path; }

private:
	void Create(uint32_t width, uint32_t height, const void* rgbaPixels);

private:
	std::string m_Path;
	uint32_t m_Width = 0;
	uint32_t m_Height = 0;
	int m_Index = 0;

	ComPtr<ID3D12Resource> m_Texture;
	uint32_t m_SRVIndex = UINT32_MAX;	// Index in the renderer's staging heap.
};
