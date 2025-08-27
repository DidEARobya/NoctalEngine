#pragma once
#include "NoctalEngine/Rendering/Bindable.h"

namespace NoctalEngine
{
	class IndexBuffer : public Bindable
	{
	public:
		virtual ~IndexBuffer() override = default;
		virtual void Bind() override {};

		virtual uint32_t GetCount() const = 0;

		virtual const BufferLayout* GetLayout() const override = 0;
	protected:
	};
}