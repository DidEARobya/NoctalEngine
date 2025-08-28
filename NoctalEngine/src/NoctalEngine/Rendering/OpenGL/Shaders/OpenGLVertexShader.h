#pragma once
#include "NoctalEngine/Rendering/Shaders/Shader.h"

class OpenGLVertexShader : public NoctalEngine::Shader
{
public:
	OpenGLVertexShader(const std::string& vertexFilePath);
	~OpenGLVertexShader();

	virtual void Bind() override {};

	virtual void SetUniformMat3(const std::string& name, const glm::mat3 value) override {};
	virtual void SetUniformMat4(const std::string& name, const glm::mat4 value) override {};
	virtual void SetUniformFloat(const std::string& name, const float value) override {};
	virtual void SetUniformFloat2(const std::string& name, const glm::vec2 value) override {};
	virtual void SetUniformFloat3(const std::string& name, const glm::vec3 value) override {};
	virtual void SetUniformFloat4(const std::string& name, const glm::vec4 value) override {};
	virtual void SetUniformInt(const std::string& name, const int value) override {};

	const uint32_t GetID() const override { return m_ShaderID; };
	const bool IsValid() const override { return m_IsValid; };

private:
	uint32_t m_ShaderID;
	bool m_IsValid;
};