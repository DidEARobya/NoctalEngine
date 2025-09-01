#pragma once
#include "NoctalEngine/Rendering/Buffers/VertexBuffer.h"
#include "NoctalEngine/Rendering/Buffers/BufferLayout.h"

#include <GLAD/glad.h>

class OpenGLVertexBuffer : public NoctalEngine::VertexBuffer
{
public:
	OpenGLVertexBuffer(const NoctalEngine::VertexBufferData& buffer);

	virtual ~OpenGLVertexBuffer() override;
	virtual void Bind() override;

	virtual void SetData(const void* data, uint32_t size) override;

	virtual size_t GetSize() override { return m_Size; };
private:
	GLenum ElementTypeToOpenGLBaseType(NoctalEngine::VertexLayout::ElementType type);

protected:
	uint32_t m_RendererID = 0;
	size_t m_Size = 0;

	NoctalEngine::VertexLayout m_Layout;
};
