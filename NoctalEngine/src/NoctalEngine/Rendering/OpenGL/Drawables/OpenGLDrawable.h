#pragma once
#include "NoctalEngine/Rendering/Drawables/Drawable.h"
#include "NoctalEngine/Rendering/Buffers/BufferLayout.h"
#include "NoctalEngine/Rendering/Geometry/Geometry.h"
#include "NoctalEngine/Rendering/OpenGL/Buffers/OpenGLUniformBufferObject.h"
#include <GLAD/glad.h>
#include <glm/glm.hpp>

class OpenGLDrawable : public Drawable
{
public:
	OpenGLDrawable(NoctalEngine::Geometry geometry);
	virtual ~OpenGLDrawable() override;

	//virtual void Update(float deltaTime) noexcept = 0;
	virtual const glm::mat4& GetTransform() const override;

	virtual void SetPosition(const glm::vec2& position) { m_Position = glm::vec3(position, 0.0f); };
	virtual glm::vec2 GetPosition() { return m_Position; };

	virtual void SetScale(const glm::vec2& scale) { m_Scale = glm::scale(glm::mat4(1.0f), { scale.x , scale.y, 1.0f }); };;

	virtual NoctalEngine::Material* GetMaterial() override { return m_Material; };

	virtual void Draw() const override;

	virtual void AddBind(std::unique_ptr<Bindable> bind) override;
	virtual void AddIndexBuffer(std::unique_ptr<NoctalEngine::IndexBuffer> indexBuffer) override;
	virtual void AddMaterial(std::unique_ptr<NoctalEngine::Material> material) override;

private:
	GLenum ShaderDataTypeToOpenGLBaseType(NoctalEngine::ShaderDataType type);

private:
	glm::vec3 m_Position;
	glm::mat4 m_Scale;

	uint32_t m_RendererID = 0;

	const NoctalEngine::IndexBuffer* m_IndexBuffer = nullptr;
	NoctalEngine::Material* m_Material = nullptr;
	std::vector<std::unique_ptr<Bindable>> m_Binds;

	std::unique_ptr<OpenGLObjectUniformBufferObject> m_ObjectBuffer = nullptr;
};