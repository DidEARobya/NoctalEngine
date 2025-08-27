#pragma once
#include "NoctalEngine/Rendering/Buffers/BufferLayout.h"

class Bindable
{
public:
	virtual ~Bindable() = default;
	virtual void Bind() = 0;

	virtual const NoctalEngine::BufferLayout* GetLayout() const = 0;
};
