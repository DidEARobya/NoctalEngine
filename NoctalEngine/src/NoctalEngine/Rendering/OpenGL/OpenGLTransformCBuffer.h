#pragma once
#include "NoctalEngine/Rendering/TransformCBuffer.h"

class Drawable;

class OpenGLTransformCBuffer : public NoctalEngine::TransformCBuffer
{
public:
	//Shader temporarily handles transform for testing
	OpenGLTransformCBuffer(const Drawable& parent);
	virtual ~OpenGLTransformCBuffer() override;
	virtual void Bind() override;

	virtual const NoctalEngine::BufferLayout* GetLayout() const override { return nullptr; };

	virtual void SetUniformMat4(const std::string& name, const glm::mat4& matrix = glm::mat4(1.0f)) override;

private:
	uint32_t m_RendererID = 0;

	struct Transform
	{
		glm::mat4 ModelTransform;
	};

	const Drawable& m_Parent;
};
