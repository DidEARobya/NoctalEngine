#pragma once
#include "OpenGLShaderProgram.h"
#include "GLAD/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "NoctalEngine/Rendering/Renderer.h"
#include "NoctalEngine/Rendering/Drawables/Drawable.h"
#include "OpenGLVertexShader.h"
#include "OpenGLFragmentShader.h"
#include "NoctalEngine/Rendering/Shaders/Shader.h"

OpenGLShaderProgram::OpenGLShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath, const Drawable& parent) : m_Parent(parent), m_IsValid(false)
{	// Link shaders together into a program.
	m_RendererID = glCreateProgram();

	std::shared_ptr<Shader> vertexShader = NoctalEngine::Renderer::Instance().GetShader(vertexShaderFilePath);

	if (vertexShader->IsValid() == false)
	{
		NE_ENGINE_ERROR("OpenGLShader Creation Failed: No valid VertexShader");
		return;
	}

	std::shared_ptr<Shader> fragmentShader = std::make_shared<OpenGLFragmentShader>(fragmentShaderFilePath);

	if (fragmentShader->IsValid() == false)
	{
		NE_ENGINE_ERROR("OpenGLShader Creation Failed: No valid FragmentShader");
		return;
	}

	glAttachShader(m_RendererID, vertexShader->GetID());
	glAttachShader(m_RendererID, fragmentShader->GetID());

	glLinkProgram(m_RendererID);

	int isLinked = 0;
	glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);

	if (isLinked == GL_FALSE)
	{
		int maxLength = 0;
		glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> infoLog(maxLength);
		glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(m_RendererID);

		NE_ENGINE_ERROR("{}", infoLog.data());
		NE_ENGINE_ASSERT(false, "Shader Program Link Failure");
		return;
	}

	glDetachShader(m_RendererID, vertexShader->GetID());
	glDetachShader(m_RendererID, fragmentShader->GetID());

	m_IsValid = true;
}

OpenGLShaderProgram::~OpenGLShaderProgram()
{
	glDeleteProgram(m_RendererID);
}

void OpenGLShaderProgram::Bind()
{
	glUseProgram(m_RendererID);
	SetUniformMat4("u_ViewProjection", NoctalEngine::Renderer::Instance().GetCameraViewProjectionMatrix());
	SetUniformMat4("u_Transform", m_Parent.GetTransform());
}

void OpenGLShaderProgram::SetUniformMat3(const std::string& name, const glm::mat3 value)
{
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShaderProgram::SetUniformMat4(const std::string& name, const glm::mat4 value)
{
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShaderProgram::SetUniformFloat(const std::string& name, const float value)
{
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1f(location, value);
}

void OpenGLShaderProgram::SetUniformFloat2(const std::string& name, const glm::vec2 value)
{
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform2f(location, value.x, value.y);
}

void OpenGLShaderProgram::SetUniformFloat3(const std::string& name, const glm::vec3 value)
{
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform3f(location, value.x, value.y, value.z);
}

void OpenGLShaderProgram::SetUniformFloat4(const std::string& name, const glm::vec4 value)
{
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void OpenGLShaderProgram::SetUniformInt(const std::string& name, const int value)
{
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1i(location, value);
}
