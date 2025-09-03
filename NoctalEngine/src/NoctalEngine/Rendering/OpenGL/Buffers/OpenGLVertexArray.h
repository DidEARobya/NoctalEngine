#pragma once
#include "NoctalEngine/Rendering/Buffers/VertexArray.h"
#include "NoctalEngine/Rendering/OpenGL/Buffers/OpenGLVertexBuffer.h"
#include "NoctalEngine/Rendering/OpenGL/Buffers/OpenGLIndexBuffer.h"
#include "NoctalEngine/Rendering/Geometry/VertexLayout.h"
#include "NoctalEngine/Rendering/RendererData.h"

class OpenGLVertexArray : public NoctalEngine::VertexArray
{
public:
	OpenGLVertexArray(const NoctalEngine::VertexBufferData& bufferData, const std::vector<uint32_t>& indices);
	virtual ~OpenGLVertexArray() override;

	virtual void Bind() override;
	virtual uint32_t GetIndices() override;
	virtual void IncrementStoredVertexData() override;
	virtual void StartBatch() override;
	virtual void Flush() override;

	GLenum ElementTypeToOpenGLBaseType(NoctalEngine::VertexLayout::ElementType type);

private:
	uint32_t m_RendererID;

	std::unique_ptr<OpenGLVertexBuffer> m_VertexBuffer = nullptr;
	std::unique_ptr<OpenGLIndexBuffer> m_IndexBuffer = nullptr;

	char* m_VertexData = nullptr;
	char* m_StoredVertexData = nullptr;

	size_t m_Stride;
	const NoctalEngine::VertexBufferData m_BufferData;

	uint32_t m_IndexCount = 0;
};