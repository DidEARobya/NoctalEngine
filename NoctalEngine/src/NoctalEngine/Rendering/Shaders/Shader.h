#pragma once
#include "NoctalEngine/Rendering/Bindable.h"
#include <glm/glm.hpp>

namespace NoctalEngine
{
	class Shader : public Bindable
	{
	public:
		virtual void SetUniformMat3(const std::string& name, const glm::mat3 value) = 0;
		virtual void SetUniformMat4(const std::string& name, const glm::mat4 value) = 0;
		virtual void SetUniformFloat(const std::string& name, const float value) = 0;
		virtual void SetUniformFloat2(const std::string& name, const glm::vec2 value) = 0;
		virtual void SetUniformFloat3(const std::string& name, const glm::vec3 value) = 0;
		virtual void SetUniformFloat4(const std::string& name, const glm::vec4 value) = 0;
		virtual void SetUniformInt(const std::string& name, const int value) = 0;
	};
}
