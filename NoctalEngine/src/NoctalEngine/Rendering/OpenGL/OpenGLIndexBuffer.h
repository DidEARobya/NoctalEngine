#pragma once
#include "NoctalEngine/Rendering/IndexBuffer.h"

	class OpenGLIndexBuffer : public NoctalEngine::IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer() override;

		virtual uint32_t GetCount() const override { return m_Count; };

		virtual void Bind() override;

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Count = 0;
	};
