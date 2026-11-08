#pragma once
#include "NoctalEngine/Rendering/Materials/Material.h"
#include "NoctalEngine/Rendering/Geometry/Geometry.h"
#include "NoctalEngine/Rendering/Geometry/Bounds.h"
#include "NoctalEngine/Rendering/RendererConfig.h"
#include <glm/glm.hpp>
#include <array>

// The local-space outline of a shape, always 4 corners so every shape fits the quad batch.
// (A triangle just repeats its last corner, which makes the second triangle of the "quad"
// zero-sized, so the GPU skips it. This lets one static index buffer serve every shape.)
struct ShapeData
{
	std::array<glm::vec2, 4> Positions;
	std::array<glm::vec2, 4> TexCoords;
};

// Something the renderer draws every frame until it's destroyed.
//
// Notice there's no GPU code in here at all. In a batch renderer a Drawable is just a
// description (where, how big, what colour/texture, which layer). The renderer gathers
// all of them and turns them into as few draw calls as possible. That's also why there's
// no OpenGLDrawable/D3D12Drawable any more: the API-specific work lives in the buffers,
// textures, pipeline and renderer instead.
class Drawable
{
public:
	Drawable(glm::vec2 position = glm::vec2(0.0f), glm::vec2 scale = glm::vec2(1.0f));
	virtual ~Drawable() = default;

	// What shape this is, and its corner positions/UVs. Implemented by DrawableBase<T>.
	virtual NoctalEngine::Geometry GetGeometry() const = 0;
	virtual const ShapeData& GetShape() const = 0;

	// ---- Transform ----
	glm::mat4 GetTransform() const;

	void SetPosition(const glm::vec2& position);
	glm::vec2 GetPosition() const { return m_Position; }

	void SetScale(const glm::vec2& scale);
	glm::vec2 GetScale() const { return m_Scale; }

	// Rotation around the centre, in radians (anti-clockwise).
	void SetRotation(float radians);
	float GetRotation() const { return m_Rotation; }

	// ---- Rendering options ----

	// Lower layers are drawn first (behind). Within a layer, objects draw in creation order.
	// See NoctalEngine::RenderLayer for some named values.
	void SetLayer(int32_t layer) { m_Layer = layer; }
	int32_t GetLayer() const { return m_Layer; }

	// Invisible drawables are skipped before culling, so they cost almost nothing.
	void SetVisible(bool visible) { m_Visible = visible; }
	bool IsVisible() const { return m_Visible; }

	NoctalEngine::Material* GetMaterial() { return &m_Material; }
	const NoctalEngine::Material& GetMaterial() const { return m_Material; }

	// ---- Used by the renderer ----

	// The 4 corners already moved into world space. Cached: only recalculated after the
	// transform changes, so static objects cost no maths per frame.
	const std::array<glm::vec3, 4>& GetWorldVertices() const;

	// World-space box around the shape, used for culling. Cached alongside the vertices.
	const NoctalEngine::AABB2D& GetBounds() const;

private:
	void UpdateWorldCache() const;

private:
	glm::vec2 m_Position;
	glm::vec2 m_Scale;
	float m_Rotation = 0.0f;

	int32_t m_Layer = NoctalEngine::RenderLayer::Default;
	bool m_Visible = true;

	NoctalEngine::Material m_Material;

	// 'mutable' lets the const getters fill the cache lazily.
	mutable std::array<glm::vec3, 4> m_WorldVertices{};
	mutable NoctalEngine::AABB2D m_Bounds;
	mutable bool m_TransformDirty = true;
};
