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

	glCreateBuffers(1, &m_InstanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_InstanceBuffer);

	// Reserve space for MaxInstances transforms
	glBufferData(GL_ARRAY_BUFFER, RendererData::MaxQuads * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

	// Setup mat4 as 4 vec4 attributes
	uint32_t attribLocation = bufferData.GetLayout().elements.size(); // next free slot
	for (uint32_t i = 0; i < 4; i++)
	{
		glEnableVertexAttribArray(attribLocation + i);
		glVertexAttribPointer(
			attribLocation + i,
			4,
			GL_FLOAT,
			GL_FALSE,
			sizeof(glm::mat4),
			(const void*)(sizeof(glm::vec4) * i)
		);

		glVertexAttribDivisor(attribLocation + i, 1); // <-- advance per instance
	}

	GLint enabled = 0, size = 0, stride = 0;
	GLenum type = 0;
	GLboolean normalized;
	GLvoid* pointer = nullptr;

	for (int i = 0; i < 8; i++) {
		glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
		if (enabled) {
			glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
			glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
			glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_TYPE, (GLint*)&type);
			glGetVertexAttribPointerv(i, GL_VERTEX_ATTRIB_ARRAY_POINTER, &pointer);
			NE_ENGINE_INFO("Attrib {} enabled: size={}, stride={}, type={}, ptr={}", i, size, stride, type, pointer);
		}
	}
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

void OpenGLVertexArray::IncrementStoredVertexData(const glm::mat4& transform)
{
	m_InstanceTransforms.push_back(transform);

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
	m_InstanceTransforms.clear();
	m_IndexCount = 0;
}

void OpenGLVertexArray::Flush()
{
	uint32_t size = (uint32_t)((uint8_t*)m_StoredVertexData - (uint8_t*)m_VertexData);
	m_VertexBuffer->SetData(m_VertexData, size);

	glBindBuffer(GL_ARRAY_BUFFER, m_InstanceBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_InstanceTransforms.size() * sizeof(glm::mat4), m_InstanceTransforms.data());

	glBindVertexArray(m_RendererID);
	NoctalEngine::Renderer::Instance().DrawIndexed(m_IndexBuffer->GetCount(), m_IndexCount, m_InstanceTransforms.size());
}
GLenum OpenGLVertexArray::ElementTypeToOpenGLBaseType(NoctalEngine::VertexLayout::ElementType type)
{
	switch (type)
	{
	case NoctalEngine::VertexLayout::ElementType::POS_2D:			return GL_FLOAT;
	case NoctalEngine::VertexLayout::ElementType::POS_3D:			return GL_FLOAT;
	case NoctalEngine::VertexLayout::ElementType::TEXCOORD:			return GL_FLOAT;
	case NoctalEngine::VertexLayout::ElementType::TRANSFORM:        return GL_FLOAT;
	}

	NE_ENGINE_ASSERT(false, "Unknown ShaderDataType");
	return 0;
}