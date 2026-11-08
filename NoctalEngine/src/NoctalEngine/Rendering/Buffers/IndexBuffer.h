#pragma once
#include "NoctalEngine/Rendering/Bindable.h"

namespace NoctalEngine
{
	// 16-bit indices use half the memory/bandwidth of 32-bit, but can only address 65535 vertices.
	enum class IndexFormat
	{
		UINT16,
		UINT32
	};

	// A GPU buffer holding indices (which vertices make up each triangle).
	// For 2D batching this is static: the quad pattern (0,1,2, 2,3,0) never changes, so it's
	// generated once at start-up and reused by every batch.
	class IndexBuffer : public Bindable
	{
	public:
		virtual ~IndexBuffer() override = default;

		virtual uint32_t GetCount() const = 0;
		virtual IndexFormat GetFormat() const = 0;
	};
}
