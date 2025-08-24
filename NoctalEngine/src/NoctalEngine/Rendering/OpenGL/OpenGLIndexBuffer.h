#pragma once
#include "NoctalEngine/Rendering/IndexBuffer.h"

namespace NoctalEngine
{
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const unsigned int indices[3]);

		virtual void Bind() override;

	private:
		unsigned int m_IndexBuffer;
	};
}