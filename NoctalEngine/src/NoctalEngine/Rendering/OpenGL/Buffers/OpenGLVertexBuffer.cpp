#include "nepch.h"
#include "OpenGLVertexBuffer.h"
#include "glad/glad.h"

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size, const NoctalEngine::BufferLayout& layout) : m_Stride(size), m_Layout(layout)
{
	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, m_Stride, vertices, GL_STATIC_DRAW);

	uint32_t index = 0;

	for (const auto& element : m_Layout)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, element.Count(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
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

GLenum OpenGLVertexBuffer::ShaderDataTypeToOpenGLBaseType(NoctalEngine::ShaderDataType type)
{
	switch (type)
	{
	case NoctalEngine::ShaderDataType::FLOAT:		    return GL_FLOAT;
	case NoctalEngine::ShaderDataType::FLOAT_2:			return GL_FLOAT;
	case NoctalEngine::ShaderDataType::FLOAT_3:			return GL_FLOAT;
	case NoctalEngine::ShaderDataType::FLOAT_4:			return GL_FLOAT;
	case NoctalEngine::ShaderDataType::MAT_3:		    return GL_FLOAT;
	case NoctalEngine::ShaderDataType::MAT_4:		    return GL_FLOAT;
	case NoctalEngine::ShaderDataType::INT:				return GL_INT;
	case NoctalEngine::ShaderDataType::INT_2:		    return GL_INT;
	case NoctalEngine::ShaderDataType::INT_3:		    return GL_INT;
	case NoctalEngine::ShaderDataType::INT_4:		    return GL_INT;
	case NoctalEngine::ShaderDataType::BOOL:		    return GL_BOOL;
	}

	NE_ENGINE_ASSERT(false, "Unknown ShaderDataType");
	return 0;
}

