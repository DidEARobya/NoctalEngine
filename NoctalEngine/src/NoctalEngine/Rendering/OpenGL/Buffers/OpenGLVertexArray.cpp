#include "nepch.h"
#include "OpenGLVertexArray.h"
#include "NoctalEngine/Rendering/RendererData.h"
#include "NoctalEngine/Rendering/Geometry/VertexLayout.h"
#include <glad/glad.h>
#include "NoctalEngine/Rendering/Geometry/Quad.h"
#include "NoctalEngine/Rendering/Geometry/IndexedTriangleList.h"
#include "NoctalEngine/Rendering/OpenGL/Drawables/OpenGLBaseObject.h"

OpenGLVertexArray::OpenGLVertexArray(const NoctalEngine::VertexBufferData& bufferData, const std::vector<uint32_t>& indices) : m_BufferData(bufferData), m_Stride(bufferData.GetLayout().Size())
{
	glCreateVertexArrays(1, &m_RendererID);
	glBindVertexArray(m_RendererID);

	size_t bufferSize = RendererData::MaxVerts * m_Stride;
	m_VertexData = new char[bufferSize];

	m_VertexBuffer = std::make_unique<OpenGLVertexBuffer>(m_BufferData);
	m_IndexBuffer = std::make_unique<OpenGLIndexBuffer>(indices, m_BufferData.Size());
}

OpenGLVertexArray::~OpenGLVertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
	delete[] m_VertexData;
}

void OpenGLVertexArray::Bind()
{
	glBindVertexArray(m_RendererID);
}

uint32_t OpenGLVertexArray::GetIndices()
{
	return m_IndexBuffer->GetCount();
}

void OpenGLVertexArray::IncrementStoredVertexData()
{
	auto shape = NoctalEngine::Quad::MakeTextured<Vertex>();
	const size_t vertexCount = shape.m_Vertices.size();

	for (size_t i = 0; i < vertexCount; i++)
	{
		Vertex vertex = shape.m_Vertices[i];
		std::memcpy(m_StoredVertexData, &vertex, m_Stride);
		m_StoredVertexData += m_Stride;
	}

	m_IndexCount += m_IndexBuffer->GetCount();
}

void OpenGLVertexArray::StartBatch()
{
	m_StoredVertexData = m_VertexData;
	m_IndexCount = 0;
}

void OpenGLVertexArray::Flush()
{
	uint32_t size = (uint32_t)((uint8_t*)m_StoredVertexData - (uint8_t*)m_VertexData);
	m_VertexBuffer->SetData(m_VertexData, size);

	glBindVertexArray(m_RendererID);
	NoctalEngine::Renderer::Instance().DrawIndexed(m_IndexBuffer->GetCount(), m_IndexCount);
}
GLenum OpenGLVertexArray::ElementTypeToOpenGLBaseType(NoctalEngine::VertexLayout::ElementType type)
{
	switch (type)
	{
	case NoctalEngine::VertexLayout::ElementType::POS_2D:			return GL_FLOAT;
	case NoctalEngine::VertexLayout::ElementType::POS_3D:			return GL_FLOAT;
	case NoctalEngine::VertexLayout::ElementType::TEXCOORD:			return GL_FLOAT;
	}

	NE_ENGINE_ASSERT(false, "Unknown ShaderDataType");
	return 0;
}