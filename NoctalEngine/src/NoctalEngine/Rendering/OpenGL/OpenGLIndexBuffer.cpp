#pragma once
#include "OpenGLIndexBuffer.h"
#include "glad/glad.h"

namespace NoctalEngine
{
	OpenGLIndexBuffer::OpenGLIndexBuffer(const unsigned int indices[3])
	{
		glGenBuffers(1, &m_IndexBuffer);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	void OpenGLIndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	}
}

