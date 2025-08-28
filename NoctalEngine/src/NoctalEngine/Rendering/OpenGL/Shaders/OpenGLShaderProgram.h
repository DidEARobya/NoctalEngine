#pragma once
#include "NoctalEngine/Rendering/Shaders/Shader.h"

class Drawable;

class OpenGLShaderProgram : public NoctalEngine::Shader
{
public:
	OpenGLShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath, const Drawable& parent);
	~OpenGLShaderProgram();

	virtual void Bind() override;

	virtual void SetUniformMat3(const std::string& name, const glm::mat3 value) override;
	virtual void SetUniformMat4(const std::string& name, const glm::mat4 value) override;
	virtual void SetUniformFloat(const std::string& name, const float value) override;
	virtual void SetUniformFloat2(const std::string& name, const glm::vec2 value) override;
	virtual void SetUniformFloat3(const std::string& name, const glm::vec3 value) override;
	virtual void SetUniformFloat4(const std::string& name, const glm::vec4 value) override;
	virtual void SetUniformInt(const std::string& name, const int value) override;

	const bool IsValid() const { return m_IsValid; };

private:
	uint32_t m_RendererID = 0;
	const Drawable& m_Parent;
	bool m_IsValid;

};
