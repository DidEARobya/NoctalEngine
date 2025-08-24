#pragma once
#include "Bindable.h"

namespace NoctalEngine
{
	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer() = default;
		unsigned int GetCount() const { return m_Count; };

	protected:
		unsigned int m_Count = 0;
	};
}