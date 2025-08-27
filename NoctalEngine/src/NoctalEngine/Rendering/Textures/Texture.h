#pragma once
#include "NoctalEngine/Rendering/Bindable.h"

namespace NoctalEngine
{
	class Texture : public Bindable
	{
	public:
		virtual ~Texture() override = default;
		virtual void Bind() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetTextureIndex(int index) = 0;

	};
}