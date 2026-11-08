#include "nepch.h"
#include "D3D12Texture2D.h"
#include "NoctalEngine/Rendering/D3D12/D3D12Renderer.h"
#include <stb_image/stb_image.h>

D3D12Texture2D::D3D12Texture2D(const std::string& path) : m_Path(path)
{
	// In D3D, UV (0,0) is the TOP-left of the image, but our shapes put UV (0,0) at their
	// BOTTOM-left corner. Flipping the rows on load makes images appear the right way up.
	stbi_set_flip_vertically_on_load(1);

	int width = 0;
	int height = 0;
	int channels = 0;

	// Ask for 4 channels (RGBA) no matter what the file has. GPUs don't do 3-channel formats
	// well, and it means every texture has the same format.
	stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
	stbi_set_flip_vertically_on_load(0);

	if (data == nullptr)
	{
		NE_ENGINE_ERROR("Failed to load texture {} - Reason: {}", path, stbi_failure_reason());

		// Fall back to a 2x2 magenta/black checker, so missing textures are obvious but harmless.
		const uint32_t missing[4] = { 0xFFFF00FF, 0xFF000000, 0xFF000000, 0xFFFF00FF };
		Create(2, 2, missing);
		return;
	}

	Create((uint32_t)width, (uint32_t)height, data);
	stbi_image_free(data);
}

D3D12Texture2D::D3D12Texture2D(uint32_t width, uint32_t height, const void* rgbaPixels)
{
	Create(width, height, rgbaPixels);
}

D3D12Texture2D::~D3D12Texture2D()
{
	// The game may destroy a texture while the GPU is still drawing with it,
	// so hand it to the renderer to release once that frame is finished.
	if (D3D12Renderer::IsActive())
	{
		D3D12Renderer& renderer = D3D12Renderer::Get();
		renderer.DeferRelease(m_Texture);
		renderer.FreeStagingDescriptor(m_SRVIndex);
	}
}

void D3D12Texture2D::Create(uint32_t width, uint32_t height, const void* rgbaPixels)
{
	D3D12Renderer& renderer = D3D12Renderer::Get();
	ID3D12Device* device = renderer.GetDevice();

	m_Width = width;
	m_Height = height;

	// ---- 1. Create the texture in GPU-only memory ----
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width = width;
	desc.Height = height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc = { 1, 0 };
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;	// Let the driver pick its fastest (swizzled) layout.
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	const D3D12_HEAP_PROPERTIES defaultHeap = D3D12Helpers::HeapProperties(D3D12_HEAP_TYPE_DEFAULT);
	NE_D3D_CHECK(device->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m_Texture)));

	if (m_Texture == nullptr)
	{
		return;
	}

	m_Texture->SetName(m_Path.empty() ? L"Texture2D" : D3D12Helpers::ToWide(m_Path).c_str());

	// ---- 2. Copy the pixels into an upload buffer ----
	// GPUs need each row of an upload to start on a 256-byte boundary, so ask D3D for the
	// exact layout ("footprint") it wants, then copy row by row.
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint = {};
	UINT rowCount = 0;
	UINT64 rowSize = 0;
	UINT64 uploadSize = 0;
	device->GetCopyableFootprints(&desc, 0, 1, 0, &footprint, &rowCount, &rowSize, &uploadSize);

	ComPtr<ID3D12Resource> upload = renderer.CreateBuffer(uploadSize, D3D12_HEAP_TYPE_UPLOAD, L"Texture Upload");

	uint8_t* mapped = nullptr;
	D3D12_RANGE readRange = { 0, 0 };
	NE_D3D_CHECK(upload->Map(0, &readRange, (void**)&mapped));

	const uint8_t* source = (const uint8_t*)rgbaPixels;
	const uint32_t sourceRowPitch = width * 4;

	for (UINT row = 0; row < rowCount; row++)
	{
		memcpy(mapped + footprint.Offset + row * footprint.Footprint.RowPitch, source + row * sourceRowPitch, sourceRowPitch);
	}

	upload->Unmap(0, nullptr);

	// ---- 3. GPU copy: upload buffer -> texture, then make it readable by pixel shaders ----
	renderer.ExecuteImmediate([&](ID3D12GraphicsCommandList* commandList)
	{
		D3D12_TEXTURE_COPY_LOCATION destination = {};
		destination.pResource = m_Texture.Get();
		destination.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		destination.SubresourceIndex = 0;

		D3D12_TEXTURE_COPY_LOCATION sourceLocation = {};
		sourceLocation.pResource = upload.Get();
		sourceLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		sourceLocation.PlacedFootprint = footprint;

		commandList->CopyTextureRegion(&destination, 0, 0, 0, &sourceLocation, nullptr);

		const D3D12_RESOURCE_BARRIER barrier = D3D12Helpers::TransitionBarrier(m_Texture.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		commandList->ResourceBarrier(1, &barrier);
	});

	// ---- 4. Create the SRV (how shaders see the texture) in the staging heap ----
	m_SRVIndex = renderer.AllocateStagingDescriptor();

	if (m_SRVIndex == UINT32_MAX)
	{
		return;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(m_Texture.Get(), &srvDesc, renderer.GetStagingCPUHandle(m_SRVIndex));
}

void D3D12Texture2D::Bind()
{
	if (m_SRVIndex == UINT32_MAX)
	{
		return;
	}

	D3D12Renderer& renderer = D3D12Renderer::Get();
	renderer.SetTextureSlot((uint32_t)m_Index, renderer.GetStagingCPUHandle(m_SRVIndex));
}
