#pragma once
#include "NoctalEngine/Rendering/Drawables/Drawable.h"
#include "NoctalEngine/Rendering/Buffers/BufferLayout.h"
#include "NoctalEngine/Rendering/OpenGL/Buffers/OpenGLUniformBufferObject.h"
#include <GLAD/glad.h>

class OpenGLDrawable : public Drawable
{
	//To implement later
	template<class T>
	friend class OpenGLDrawableBase;

public:
	virtual ~OpenGLDrawable() override;

	virtual NoctalEngine::Material* GetMaterial() override { return m_Material; };

	virtual void Draw() const override;

	virtual void AddBind(std::unique_ptr<Bindable> bind) override;
	virtual void SetIndexBuffer(std::unique_ptr<NoctalEngine::IndexBuffer> indexBuffer) override;
	virtual void SetMaterial(std::unique_ptr<NoctalEngine::Material> material) override;

private:
	GLenum ShaderDataTypeToOpenGLBaseType(NoctalEngine::ShaderDataType type);
	//To implement later
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;

protected:
	uint32_t m_RendererID = 0;

	const NoctalEngine::IndexBuffer* m_IndexBuffer = nullptr;
	NoctalEngine::Material* m_Material = nullptr;
	std::vector<std::unique_ptr<Bindable>> m_Binds;

	std::unique_ptr<OpenGLObjectUniformBufferObject> m_ObjectBuffer = nullptr;
};