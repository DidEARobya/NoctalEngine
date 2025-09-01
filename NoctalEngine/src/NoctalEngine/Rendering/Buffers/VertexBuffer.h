#pragma once
#include "NoctalEngine/Rendering/Bindable.h"
#include "NoctalEngine/Rendering/Geometry/VertexLayout.h"

namespace NoctalEngine
{
	class VertexBuffer : public Bindable
	{
	public:
		virtual void SetData(const void* data, uint32_t size) = 0;
		virtual size_t GetSize() = 0;
	};
}