#pragma once
#include "NoctalEngine/Rendering/Bindable.h"

namespace NoctalEngine
{
	class Shader : public Bindable
	{
		virtual const BufferLayout* GetLayout() const override = 0;
	};
}
