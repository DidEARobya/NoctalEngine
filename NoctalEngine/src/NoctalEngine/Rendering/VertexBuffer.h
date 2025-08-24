#pragma once
#include "Bindable.h"

namespace NoctalEngine
{
	class VertexBuffer : public Bindable
	{
	public:
		VertexBuffer() = default;

	protected:
		unsigned int m_Stride = 0;
	};
}