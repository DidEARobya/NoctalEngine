#pragma once
#include "NoctalEngine/Rendering/Bindable.h"
#include "NoctalEngine/Rendering/Buffers/BufferLayout.h"

namespace NoctalEngine
{
	// A GPU buffer holding vertices.
	// This one is "dynamic": the batch renderer refills it every frame (often several times a frame).
	// Each API decides how to make that safe while the GPU might still be reading older data.
	class VertexBuffer : public Bindable
	{
	public:
		virtual ~VertexBuffer() override = default;

		// Copy 'size' bytes into the buffer. The data is used by the next draw after Bind().
		virtual void SetData(const void* data, uint32_t size) = 0;

		// Describes what one vertex looks like (position, colour, ...). Used to build the pipeline.
		virtual const BufferLayout& GetLayout() const = 0;
	};
}
