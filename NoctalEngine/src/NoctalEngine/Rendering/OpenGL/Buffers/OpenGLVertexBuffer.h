#pragma once
#include "NoctalEngine/Rendering/Buffers/VertexBuffer.h"
#include "NoctalEngine/Rendering/Buffers/BufferLayout.h"

#include <GLAD/glad.h>

class OpenGLVertexBuffer : public NoctalEngine::VertexBuffer
{
public:
	template<class V>
	OpenGLVertexBuffer(const std::vector<V>& vertices, const NoctalEngine::BufferLayout& layout) : m_Stride(sizeof(V) * (uint32_t)vertices.size()), m_Layout(layout)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, m_Stride, vertices.data(), GL_DYNAMIC_DRAW);

		uint32_t index = 0;

		for (const auto& element : m_Layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.Count(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)(uintptr_t)element.Offset);
			index++;
		}
	}
	virtual ~OpenGLVertexBuffer() override;
	virtual void Bind() override;

private:
	GLenum ShaderDataTypeToOpenGLBaseType(NoctalEngine::ShaderDataType type);

protected:
	uint32_t m_RendererID = 0;
	uint32_t m_Stride = 0;

	NoctalEngine::BufferLayout m_Layout;
};
