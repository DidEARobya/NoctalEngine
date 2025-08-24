#pragma once
#include "NoctalEngine/Rendering/Bindable.h"


	class OpenGLShader : public Bindable
	{
	public:
		OpenGLShader(const std::string& vertexSource, const std::string& pixelSource);
		~OpenGLShader();

		virtual void Bind() override;

	private:
		uint32_t m_RendererID;
	};
