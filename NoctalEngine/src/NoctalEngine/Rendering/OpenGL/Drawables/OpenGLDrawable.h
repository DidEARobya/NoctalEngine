#pragma once
#include "NoctalEngine/Rendering/Drawables/Drawable.h"
#include "NoctalEngine/Rendering/Buffers/BufferLayout.h"
#include "NoctalEngine/Rendering/Geometry/Geometry.h"
#include "GLAD/glad.h"
#include "glm/glm.hpp"

class OpenGLDrawable : public Drawable
{
public:
	OpenGLDrawable(NoctalEngine::Geometry geometry);
	virtual ~OpenGLDrawable() override;

	//virtual void Update(float deltaTime) noexcept = 0;
	virtual const glm::mat4& GetTransform() const override;

	virtual void SetPosition(const glm::vec3& position) { m_Position = position; };
	virtual glm::vec3 GetPosition() { return m_Position; };

	virtual void SetScale(const glm::mat4& scale) { m_Scale = scale; };;

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

	const class NoctalEngine::IndexBuffer* m_IndexBuffer = nullptr;
	class NoctalEngine::Material* m_Material = nullptr;
	std::vector<std::unique_ptr<Bindable>> m_Binds;
};