#include "nepch.h"
#include "OpenGLVertexBuffer.h"

OpenGLVertexBuffer::OpenGLVertexBuffer(const NoctalEngine::VertexBufferData& buffer) : m_Size(buffer.SizeBytes()), m_Layout({
		{ NoctalEngine::ShaderDataType::POS_3D, "a_Position"},
		{ NoctalEngine::ShaderDataType::POS_2D, "a_TexCoord"},
	})
{
	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, m_Size, buffer.GetData(), GL_DYNAMIC_DRAW);

	uint32_t index = 0;

	for (const auto& element : buffer.GetLayout().elements)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, element.Count(), ElementTypeToOpenGLBaseType(element.GetType()), GL_FALSE, buffer.GetLayout().Size(), (const void*)(uintptr_t)element.GetOffset());
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

