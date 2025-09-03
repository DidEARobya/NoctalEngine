#include "nepch.h"
#include "OpenGLIndexBuffer.h"
#include "NoctalEngine/Rendering/Renderer.h"
#include "NoctalEngine/Rendering/RendererData.h"
#include "glad/glad.h"

OpenGLIndexBuffer::OpenGLIndexBuffer(const std::vector<uint32_t>& indices, const uint32_t vertsCount) : m_Count((uint32_t)indices.size())
{
	std::vector<uint32_t> newIndices(RendererData::MaxIndices);
	uint32_t primitiveCount = RendererData::MaxIndices / m_Count;
	uint32_t offset = 0;

	for (uint32_t p = 0; p < primitiveCount; p++)
	{
		for (uint32_t i = 0; i < m_Count; i++)
		{
			newIndices[p * m_Count + i] = indices[i] + offset;
		}

		NE_ENGINE_INFO(vertsCount);
		offset += vertsCount;
	}

	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, newIndices.size() * sizeof(uint32_t), newIndices.data(), GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void OpenGLIndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

