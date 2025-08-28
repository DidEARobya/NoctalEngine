#include "OpenGLFragmentShader.h"
#include "GLAD/glad.h"
#include <fstream>

OpenGLFragmentShader::OpenGLFragmentShader(const std::string& fragmentFilePath) : m_ShaderID(0), m_IsValid(false)
{
	std::string fragmentSource;
	std::ifstream in(fragmentFilePath, std::ios::in | std::ios::binary);

	if (in)
	{
		in.seekg(0, std::ios::end);
		size_t size = in.tellg();
		fragmentSource.resize(size);

		in.seekg(0, std::ios::beg);
		in.read(&fragmentSource[0], size);  // read the file contents into the string buffer

		in.close();

		m_ShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		const char* source = fragmentSource.c_str();
		glShaderSource(m_ShaderID, 1, &source, 0);

		glCompileShader(m_ShaderID);

		int isCompiled = 0;
		glGetShaderiv(m_ShaderID, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE)
		{
			int maxLength = 0;
			glGetShaderiv(m_ShaderID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(m_ShaderID, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(m_ShaderID);

			NE_ENGINE_ERROR("{}", infoLog.data());
			NE_ENGINE_ASSERT(false, "Vertex Shader Compilation Failure");
			return;
		}

		m_IsValid = true;
	}
	else
	{
		NE_ENGINE_ERROR("Failed to load Shader File: {}", fragmentFilePath);
	}
}

OpenGLFragmentShader::~OpenGLFragmentShader()
{
	glDeleteShader(m_ShaderID);
}
