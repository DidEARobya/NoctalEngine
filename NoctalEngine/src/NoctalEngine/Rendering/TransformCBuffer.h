#pragma once
#include "Bindable.h"
#include "glm/glm.hpp"

namespace NoctalEngine
{
	class TransformCBuffer : public Bindable
	{
	public:
		virtual ~TransformCBuffer() override = default;
		virtual void Bind() override {};

		virtual void SetUniformMat4(const std::string& name, const glm::mat4& matrix) = 0;

		virtual const BufferLayout* GetLayout() const override = 0;
	};
}