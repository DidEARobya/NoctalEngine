#pragma once
#include "Bindable.h"

namespace NoctalEngine
{
	class IndexBuffer : public Bindable
	{
	public:
		virtual ~IndexBuffer() {};
		virtual uint32_t GetCount() const = 0;

	protected:
	};
}