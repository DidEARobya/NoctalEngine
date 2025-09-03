#pragma once
#include "NoctalEngine/Rendering/Buffers/IndexBuffer.h"

	class OpenGLIndexBuffer : public NoctalEngine::IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const std::vector<uint32_t>& indices, const uint32_t vertsCount);
		virtual ~OpenGLIndexBuffer() override;

		virtual uint32_t GetCount() const override { return m_Count; };

		virtual void Bind() override;

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Count = 0;
	};
