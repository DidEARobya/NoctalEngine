#pragma once
#include "NoctalEngine/Rendering/Buffers/VertexArray.h"
#include "NoctalEngine/Rendering/Buffers/VertexBuffer.h"
#include "NoctalEngine/Rendering/Buffers/IndexBuffer.h"
#include "NoctalEngine/Rendering/OpenGL/Buffers/OpenGLVertexBuffer.h"
#include "NoctalEngine/Rendering/OpenGL/Buffers/OpenGLIndexBuffer.h"
#include "NoctalEngine/Rendering/Geometry/VertexLayout.h"
#include "NoctalEngine/Rendering/Geometry/IndexedTriangleList.h"
#include <glad/glad.h>

class OpenGLVertexArray : public NoctalEngine::VertexArray
{
public:
	template<class V>
	OpenGLVertexArray(const NoctalEngine::VertexBufferData& bufferData, const IndexedTriangleList<V> vertices)
	{
        glCreateVertexArrays(1, &m_RendererID);
        glBindVertexArray(m_RendererID);

        m_VertexBuffer = std::make_unique<OpenGLVertexBuffer>(bufferData);

        m_IndexBuffer = std::make_unique<OpenGLIndexBuffer>(vertices.m_Indices);

		glBindVertexArray(0);

	}

	virtual ~OpenGLVertexArray() override;

	virtual void Bind() override;
	virtual void IncrementIndicesCount(const uint32_t count) override { m_IndicesUsed += count; };
	virtual uint32_t GetIndicesTotal() override;
	virtual void ResetCount() override { m_IndicesUsed = 0; };
	virtual uint32_t GetIndices();

	virtual size_t GetVertexBufferSize() override { return m_VertexBuffer->GetSize(); };
	virtual void SetData(const void* data, uint32_t size) override {
		m_VertexBuffer->SetData(data, size);
	};
private:
	GLenum ElementTypeToOpenGLBaseType(NoctalEngine::VertexLayout::ElementType type);

private:
	uint32_t m_RendererID = 0;
	uint32_t m_IndicesUsed = 0;

	std::unique_ptr<NoctalEngine::VertexBuffer> m_VertexBuffer = nullptr;
	std::unique_ptr<NoctalEngine::IndexBuffer> m_IndexBuffer = nullptr;
};