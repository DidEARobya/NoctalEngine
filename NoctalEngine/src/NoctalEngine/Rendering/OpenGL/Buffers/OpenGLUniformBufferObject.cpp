#include "nepch.h"
#include "OpenGLUniformBufferObject.h"
#include "glad/glad.h"

OpenGLUniformBufferObject::OpenGLUniformBufferObject(int bindingPoint, uint32_t size) : m_BindPoint(bindingPoint), m_Size(size)
{
    glGenBuffers(1, &m_UniformBufferObjectID);
    glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferObjectID);
    glBufferData(GL_UNIFORM_BUFFER, m_Size, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_UniformBufferObjectID);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

OpenGLUniformBufferObject::~OpenGLUniformBufferObject()
{
    glDeleteBuffers(1, &m_UniformBufferObjectID);
}

void OpenGLUniformBufferObject::UploadUniforms(const void* data, size_t dataSize, int offset)
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferObjectID);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, dataSize, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

//Frame Buffer
OpenGLFrameUniformBufferObject::OpenGLFrameUniformBufferObject() : OpenGLUniformBufferObject(0, sizeof(FrameData)), m_Data({ glm::mat4{0.0f} })
{

}

void OpenGLFrameUniformBufferObject::Bind()
{
    UploadUniforms(&m_Data, m_Size);
}

void OpenGLFrameUniformBufferObject::UpdateFrameData(FrameData& data)
{
    m_Data.ViewProjection = data.ViewProjection;
}

// Material Buffer
OpenGLMaterialUniformBufferObject::OpenGLMaterialUniformBufferObject() : OpenGLUniformBufferObject(1, sizeof(MaterialData)), m_Data({ {0.0f, 0.0f, 0.0f, 0.0f} })
{

}

void OpenGLMaterialUniformBufferObject::Bind()
{
    UploadUniforms(&m_Data, m_Size);
}

void OpenGLMaterialUniformBufferObject::UpdateMaterialData(MaterialData& data)
{
    data.Colour = data.Colour;
}

// Object Buffer
OpenGLObjectUniformBufferObject::OpenGLObjectUniformBufferObject() : OpenGLUniformBufferObject(2, sizeof(ObjectData)), m_Data({ glm::mat4{0.0f} })
{
}

void OpenGLObjectUniformBufferObject::Bind()
{
    UploadUniforms(&m_Data, m_Size);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_UniformBufferObjectID);
}

void OpenGLObjectUniformBufferObject::UpdateObjectData(ObjectData& data)
{
    m_Data.Transform = data.Transform;
}
