#pragma once
#include "NoctalEngine/Rendering/VertexBuffer.h"

namespace NoctalEngine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const float vertices[3 * 3]);
		virtual void Bind() override;

	protected:
		unsigned int m_VertexArray;
		unsigned int m_VertexBuffer;

		unsigned int m_Stride;
	};
}