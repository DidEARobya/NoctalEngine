#pragma once
#include "NoctalEngine/Rendering/Shader.h"


	class OpenGLShader : public NoctalEngine::Shader
	{
	public:
		OpenGLShader(const std::string& vertexSource, const std::string& pixelSource);
		~OpenGLShader();

		virtual void Bind() override;
		virtual void SetUniformMat4(const std::string& name, const glm::mat4 matrix) override;

		virtual const NoctalEngine::BufferLayout* GetLayout() const override { return nullptr; };

	private:
		uint32_t m_RendererID = 0;
	};
