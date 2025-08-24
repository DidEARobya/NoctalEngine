#pragma once
#include "NoctalEngine/Rendering/IndexBuffer.h"

	class OpenGLIndexBuffer : public NoctalEngine::IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const unsigned int indices[3]);

		virtual void Bind() override;

	private:
		unsigned int m_IndexBuffer;
	};
