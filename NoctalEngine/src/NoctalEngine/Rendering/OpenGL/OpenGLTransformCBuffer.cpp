#pragma once
#include "OpenGLTransformCBuffer.h"
#include "NoctalEngine/Rendering/Drawables/Drawable.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

OpenGLTransformCBuffer::OpenGLTransformCBuffer(const Drawable& parent) : m_Parent(parent)
{
}

OpenGLTransformCBuffer::~OpenGLTransformCBuffer()
{
}

void OpenGLTransformCBuffer::Bind()
{
	SetUniformMat4("u_Transform", m_Parent.GetTransform());
}

void OpenGLTransformCBuffer::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
{
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
