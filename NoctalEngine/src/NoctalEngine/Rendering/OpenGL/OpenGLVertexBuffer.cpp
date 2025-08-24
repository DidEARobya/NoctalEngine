#pragma once
#include "OpenGLVertexBuffer.h"
#include "glad/glad.h"

OpenGLVertexBuffer::OpenGLVertexBuffer(const float vertices[3 * 3]) : m_Stride(sizeof(vertices))
{
	glGenVertexArrays(1, &m_VertexArray);
	glGenBuffers(1, &m_VertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, m_Stride, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
}

void OpenGLVertexBuffer::Bind()
{
	glBindVertexArray(m_VertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
}