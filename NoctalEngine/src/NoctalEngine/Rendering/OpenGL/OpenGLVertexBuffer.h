#pragma once
#include "NoctalEngine/Rendering/VertexBuffer.h"
#include "NoctalEngine/Rendering/BufferLayout.h"
#include "GLAD/glad.h"

class OpenGLVertexBuffer : public NoctalEngine::VertexBuffer
{
public:
	OpenGLVertexBuffer(float* vertices, uint32_t size, const NoctalEngine::BufferLayout& layout);
	virtual ~OpenGLVertexBuffer() override;

	virtual void Bind() override;

	virtual const NoctalEngine::BufferLayout& GetBufferLayout() const { return m_Layout; };

private:
	GLenum ShaderDataTypeToOpenGLBaseType(NoctalEngine::ShaderDataType type);

protected:
	uint32_t m_RendererID = 0;
	uint32_t m_Stride = 0;

	NoctalEngine::BufferLayout m_Layout;
};