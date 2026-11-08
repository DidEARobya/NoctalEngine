#pragma once
#include <cstdint>

// ---------------------------------------------------------------------------------
// RendererConfig
// One place to tune the renderer's limits. These are API-agnostic: every backend
// (D3D12 now, OpenGL/Vulkan later) must respect them.
// ---------------------------------------------------------------------------------
namespace NoctalEngine
{
	namespace RendererConfig
	{
		// How many quads fit in a single draw call.
		// Kept <= 16383 so (quads * 4) vertices fit in 16-bit indices (max 65535),
		// which halves the index buffer size compared to 32-bit indices.
		constexpr uint32_t MaxQuadsPerBatch = 10000;
		constexpr uint32_t MaxVerticesPerBatch = MaxQuadsPerBatch * 4;
		constexpr uint32_t MaxIndicesPerBatch = MaxQuadsPerBatch * 6;

		// Upper limit of quads drawn in one frame (across all batches).
		// The dynamic vertex buffer is sized from this.
		constexpr uint32_t MaxQuadsPerFrame = 100000;

		// How many unique textures one batch can reference.
		// The pixel shader declares a texture array of exactly this size.
		// Slot 0 is always a 1x1 white texture (used for untextured, coloured quads).
		constexpr uint32_t MaxTextureSlots = 32;
	}

	// Named render layers. Lower values are drawn first (further "behind").
	// These are just ints, so any value in between is valid too (e.g. Default + 5).
	namespace RenderLayer
	{
		constexpr int32_t Background = -100;
		constexpr int32_t Default    = 0;
		constexpr int32_t Foreground = 100;
		constexpr int32_t Overlay    = 200;
	}
}
