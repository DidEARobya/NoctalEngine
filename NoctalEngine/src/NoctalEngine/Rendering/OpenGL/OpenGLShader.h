#pragma once
#include "NoctalEngine/Rendering/Shader.h"


	class OpenGLShader : public NoctalEngine::Shader
	{
	public:
		OpenGLShader(const std::string& vertexSource, const std::string& pixelSource);
		~OpenGLShader();

		virtual void Bind() override;

	private:
		uint32_t m_RendererID = 0;
	};
