#pragma once
#include "NoctalEngine/Rendering/Bindable.h"
#include <glm/glm.hpp>

namespace NoctalEngine
{
	class Shader : public Bindable
	{
		virtual const BufferLayout* GetLayout() const override = 0;
		virtual void SetUniformMat4(const std::string& name, const glm::mat4 matrix) = 0;

	};
}
