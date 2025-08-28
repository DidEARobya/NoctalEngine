#pragma once

class OpenGLVertexShader
{
public:
	OpenGLVertexShader(const std::string& vertexFilePath);
	~OpenGLVertexShader();

	const uint32_t GetID() const { return m_ShaderID; };
	const bool IsValid() const { return m_IsValid; };

private:
	uint32_t m_ShaderID;
	bool m_IsValid;
};