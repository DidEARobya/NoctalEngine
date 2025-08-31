#include "nepch.h"
#include "OpenGLVertexBuffer.h"

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

