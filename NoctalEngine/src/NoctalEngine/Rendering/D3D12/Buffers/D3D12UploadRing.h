#pragma once
#include "NoctalEngine/Rendering/D3D12/D3D12Common.h"

// ---------------------------------------------------------------------------------
// D3D12UploadRing
// CPU-writable GPU memory for data that changes every frame (vertices, constants).
//
// The problem: when the CPU writes frame N+1's vertices, the GPU may still be reading
// frame N's. Overwriting them would corrupt frame N.
//
// The fix: one buffer split into FramesInFlight sections. Each frame writes only into its
// own section, moving forward (linear allocation) with every SetData() call.
// By the time we come back round to a section, the renderer has waited for the GPU to
// finish that frame, so it's safe to start again from the beginning.
//
//   [ frame 0 section ][ frame 1 section ]
//     ^ alloc, alloc, alloc ->
// ---------------------------------------------------------------------------------
class D3D12UploadRing
{
public:
	struct Allocation
	{
		void* CPU = nullptr;						// Where to memcpy into.
		D3D12_GPU_VIRTUAL_ADDRESS GPU = 0;			// The same memory, as the GPU sees it.
		bool IsValid() const { return CPU != nullptr; }
	};

	~D3D12UploadRing();

	void Create(uint64_t bytesPerFrame, const wchar_t* debugName);
	void Destroy();

	// Returns an invalid allocation if this frame's section is full.
	Allocation Allocate(uint64_t size, uint64_t alignment);

private:
	ComPtr<ID3D12Resource> m_Buffer;
	uint8_t* m_MappedData = nullptr;			// Stays mapped for the buffer's lifetime ("persistent mapping").
	D3D12_GPU_VIRTUAL_ADDRESS m_GPUAddress = 0;

	uint64_t m_BytesPerFrame = 0;
	uint64_t m_Offset = 0;						// How far into this frame's section we are.
	uint64_t m_LastFrameNumber = UINT64_MAX;	// Used to notice a new frame has started.
};
