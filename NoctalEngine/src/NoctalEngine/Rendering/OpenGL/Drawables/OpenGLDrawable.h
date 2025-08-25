#pragma once
#include "NoctalEngine/Rendering/Drawables/Drawable.h"
#include "NoctalEngine/Rendering/BufferLayout.h"
#include "NoctalEngine/Rendering/Geometry/Geometry.h"
#include "GLAD/glad.h"

class OpenGLDrawable : public Drawable
{
public:
	OpenGLDrawable(NoctalEngine::Geometry geometry);
	virtual ~OpenGLDrawable() override;

	//virtual void Update(float deltaTime) noexcept = 0;

	virtual void Draw() const override;

	virtual void AddBind(std::unique_ptr<Bindable> bind) override;
	virtual void AddIndexBuffer(std::unique_ptr<NoctalEngine::IndexBuffer> indexBuffer) override;

private:
	GLenum ShaderDataTypeToOpenGLBaseType(NoctalEngine::ShaderDataType type);

private:
	uint32_t m_RendererID = 0;

	const class NoctalEngine::IndexBuffer* m_IndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> m_Binds;
};