#include "nepch.h"
#include "OpenGLIndexBuffer.h"
#include "NoctalEngine/Rendering/Renderer.h"
#include "glad/glad.h"

OpenGLIndexBuffer::OpenGLIndexBuffer(const std::vector<uint32_t>& indices) : m_Count((uint32_t)indices.size())
{
	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void OpenGLIndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	NoctalEngine::Renderer::Instance().SetIndexBuffer(this);
}

