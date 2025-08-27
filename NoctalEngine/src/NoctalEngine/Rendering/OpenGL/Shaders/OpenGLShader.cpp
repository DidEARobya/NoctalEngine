#pragma once
#include "OpenGLShader.h"
#include "GLAD/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "NoctalEngine/Rendering/Renderer.h"
#include "NoctalEngine/Rendering/Drawables/Drawable.h"

OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& pixelSource, const Drawable& parent) : m_Parent(parent)
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Send the vertex shader source code to GL
	const char* source = vertexSource.c_str();
	glShaderSource(vertexShader, 1, &source, 0);

	glCompileShader(vertexShader);

	int isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE)
	{
		int maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(vertexShader);

		NE_ENGINE_ERROR("{}", infoLog.data());
		NE_ENGINE_ASSERT(false, "Vertex Shader Compilation Failure");
		return;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Send the pixel shader source code to GL
	source = pixelSource.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);

	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		int maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);

		NE_ENGINE_ERROR("{}", infoLog.data());
		NE_ENGINE_ASSERT(false, "Pixel Shader Compilation Failure");
		return;
	}

	// Link shaders together into a program.
	m_RendererID = glCreateProgram();

	glAttachShader(m_RendererID, vertexShader);
	glAttachShader(m_RendererID, fragmentShader);

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
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		NE_ENGINE_ERROR("{}", infoLog.data());
		NE_ENGINE_ASSERT(false, "Shader Program Link Failure");
		return;
	}

	glDetachShader(m_RendererID, vertexShader);
	glDetachShader(m_RendererID, fragmentShader);
}

OpenGLShader::~OpenGLShader()
{
	glDeleteProgram(m_RendererID);
}

void OpenGLShader::Bind()
{
	glUseProgram(m_RendererID);
	SetUniformMat4("u_ViewProjection", NoctalEngine::Renderer::Instance().GetCameraViewProjectionMatrix());
	SetUniformMat4("u_Transform", m_Parent.GetTransform());
}

void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3 value)
{
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4 value)
{
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::SetUniformFloat(const std::string& name, const float value)
{
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1f(location, value);
}

void OpenGLShader::SetUniformFloat2(const std::string& name, const glm::vec2 value)
{
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform2f(location, value.x, value.y);
}

void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3 value)
{
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform3f(location, value.x, value.y, value.z);
}

void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4 value)
{
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::SetUniformInt(const std::string& name, const int value)
{
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1i(location, value);
}
