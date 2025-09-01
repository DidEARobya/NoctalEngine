#include "OpenGLVertexArray.h"

OpenGLVertexArray::~OpenGLVertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void OpenGLVertexArray::Bind()
{
	glBindVertexArray(m_RendererID);
}

uint32_t OpenGLVertexArray::GetIndicesTotal()
{
	return m_IndicesUsed;
}

uint32_t OpenGLVertexArray::GetIndices()
{
	return m_IndexBuffer->GetCount();
}

GLenum OpenGLVertexArray::ElementTypeToOpenGLBaseType(NoctalEngine::VertexLayout::ElementType type)
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