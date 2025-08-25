#pragma once
#include "Bindable.h"

namespace NoctalEngine
{
	class VertexBuffer : public Bindable
	{
	public:
		virtual ~VertexBuffer() override = default;
		virtual void Bind() override {};

		virtual const BufferLayout* GetLayout() const override = 0;
	};
}