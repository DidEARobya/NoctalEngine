#pragma once
#include "NoctalEngine/Rendering/Bindable.h"
#include <glm/glm.hpp>

namespace NoctalEngine
{
	class VertexArray : public Bindable
	{
	public:
		virtual void StartBatch() = 0;
		virtual void Flush() = 0;
		virtual void IncrementStoredVertexData(const glm::mat4& transform) = 0;

		virtual uint32_t GetIndices() = 0;
	};
}
