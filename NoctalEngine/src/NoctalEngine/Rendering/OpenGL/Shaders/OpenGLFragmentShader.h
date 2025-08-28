#pragma once

class OpenGLFragmentShader
{
public:
	OpenGLFragmentShader(const std::string& fragmentFilePath);
	~OpenGLFragmentShader();

	const uint32_t GetID() const { return m_ShaderID; };
	const bool IsValid() const { return m_IsValid; };

private:
	uint32_t m_ShaderID;
	bool m_IsValid;
};