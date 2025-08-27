#pragma once
#include "NoctalEngine/Rendering/Buffers/BufferLayout.h"

class Bindable
{
public:
	virtual ~Bindable() = default;
	virtual void Bind() = 0;
};
