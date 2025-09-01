#include "nepch.h"
#include "OpenGLVertexBuffer.h"
#include "NoctalEngine/Rendering/RendererData.h"

OpenGLVertexBuffer::OpenGLVertexBuffer(const NoctalEngine::VertexBufferData& bufferData) : m_Size(bufferData.SizeBytes()), m_Layout(bufferData.GetLayout())
{
	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

	glBufferData(GL_ARRAY_BUFFER, RendererData::Instance().MaxVerts * bufferData.GetLayout().Size(), nullptr, GL_DYNAMIC_DRAW);

	RendererData::Instance().QuadVertexSize = m_Layout.Size();
	RendererData::Instance().QuadVertexBufferBase = new uint8_t[RendererData::Instance().MaxVerts * RendererData::Instance().QuadVertexSize];

	uint32_t index = 0;
	for (const auto& element : m_Layout.GetElements())
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, element.Count(), ElementTypeToOpenGLBaseType(element.GetType()), GL_FALSE, (GLsizei)m_Layout.Size(), (const void*)(uintptr_t)element.GetOffset());
		index++;
	}
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void OpenGLVertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

GLenum OpenGLVertexBuffer::ElementTypeToOpenGLBaseType(NoctalEngine::VertexLayout::ElementType type)
{
	switch (type)
	{
		case NoctalEngine::VertexLayout::ElementType::POS_2D:			return GL_FLOAT;
		case NoctalEngine::VertexLayout::ElementType::POS_3D:			return GL_FLOAT;
		case NoctalEngine::VertexLayout::ElementType::TEXCOORD:			return GL_FLOAT;
	}

	NE_ENGINE_ASSERT(false, "Unknown ShaderDataType");
	return 0;
}

