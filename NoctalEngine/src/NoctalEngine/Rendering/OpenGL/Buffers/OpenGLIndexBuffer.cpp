#include "nepch.h"
#include "OpenGLIndexBuffer.h"
#include "NoctalEngine/Rendering/Renderer.h"
#include "NoctalEngine/Rendering/RendererData.h"
#include "glad/glad.h"

OpenGLIndexBuffer::OpenGLIndexBuffer(const std::vector<uint32_t>& indices) : m_Count(RendererData::Instance().MaxIndices)
{
    std::vector<uint32_t> temp;
    temp.reserve(m_Count);

    uint32_t offset = 0;
    for (uint32_t i = 0; i < RendererData::Instance().MaxIndices; i += 6)
    {
        temp.push_back(offset + 0);
        temp.push_back(offset + 1);
        temp.push_back(offset + 2);
        temp.push_back(offset + 2);
        temp.push_back(offset + 3);
        temp.push_back(offset + 0);

        offset += 4; // next quad’s vertices
    }

    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, temp.size() * sizeof(uint32_t), temp.data(), GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void OpenGLIndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

