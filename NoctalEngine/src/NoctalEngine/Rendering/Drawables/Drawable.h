#pragma once
#include "NoctalEngine/Rendering/Bindable.h"
#include "NoctalEngine/Rendering/Buffers/IndexBuffer.h"
#include "NoctalEngine/Rendering/Materials/Material.h"
#include "NoctalEngine/Rendering/Geometry/Geometry.h"
#include "glm/glm.hpp"

class Drawable
{
public:
	Drawable() = default;
	virtual ~Drawable() = default;

	//virtual void Update(float deltaTime) noexcept = 0;
	virtual glm::mat4 GetTransform() const = 0;

	virtual void SetPosition(const glm::vec2& position) = 0;
	virtual glm::vec2 GetPosition() = 0;

	virtual void SetScale(const glm::vec2& scale) = 0;

	virtual NoctalEngine::Material* GetMaterial() = 0;
	virtual void Draw() const = 0;

	virtual void AddBind(std::unique_ptr<Bindable> bind) = 0;
	virtual void SetMaterial(std::unique_ptr<NoctalEngine::Material> material) = 0;

	virtual NoctalEngine::Geometry GetGeometry() = 0;
};