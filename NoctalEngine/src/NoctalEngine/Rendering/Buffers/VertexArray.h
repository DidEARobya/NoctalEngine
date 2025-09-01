#pragma once
#include "NoctalEngine/Rendering/Bindable.h"

namespace NoctalEngine
{
	class VertexArray : public Bindable
	{
	public:
		virtual void IncrementIndicesCount(const uint32_t count) = 0;
		virtual uint32_t GetIndicesTotal() = 0;
		virtual uint32_t GetIndices() = 0;
		virtual void ResetCount() = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;
		virtual size_t GetVertexBufferSize() = 0;
	};
}