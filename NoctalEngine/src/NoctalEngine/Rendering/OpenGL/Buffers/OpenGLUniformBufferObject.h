#pragma once
#include "NoctalEngine/Rendering/Buffers/ConstantBuffer.h"
#include <glm/gtc/type_ptr.hpp>

class OpenGLUniformBufferObject : public NoctalEngine::ConstantBuffer
{
public:
    OpenGLUniformBufferObject(int bindingPoint, uint32_t size);
    ~OpenGLUniformBufferObject();

protected:
    void UploadUniforms(const void* data, size_t dataSize, int offset = 0);

protected:
    uint32_t m_UniformBufferObjectID = 0;
    int m_BindPoint = 0;
    uint32_t m_Size = 0;
};

struct FrameData
{
    glm::mat4 ViewProjection;
};

class OpenGLFrameUniformBufferObject : public OpenGLUniformBufferObject
{
public:
    OpenGLFrameUniformBufferObject();

    virtual void Bind() override;
    void UpdateFrameData(FrameData& data);

private:
    FrameData m_Data;
};

struct MaterialData
{
    glm::vec4 Colour;
};

class OpenGLMaterialUniformBufferObject : public OpenGLUniformBufferObject
{
public:
    OpenGLMaterialUniformBufferObject();

    virtual void Bind() override;
    void UpdateMaterialData(MaterialData& data);

private:
    MaterialData m_Data;
};

struct ObjectData
{
    glm::mat4 Transform;
};

class OpenGLObjectUniformBufferObject : public OpenGLUniformBufferObject
{
public:
    OpenGLObjectUniformBufferObject();

    virtual void Bind() override;
    void UpdateObjectData(ObjectData& data);

private:
    ObjectData m_Data;
};