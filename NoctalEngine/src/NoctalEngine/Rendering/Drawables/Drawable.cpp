#include "nepch.h"
#include "Drawable.h"
#include <glm/gtc/matrix_transform.hpp>

Drawable::Drawable(glm::vec2 position, glm::vec2 scale) : m_Position(position), m_Scale(scale)
{
}

glm::mat4 Drawable::GetTransform() const
{
	// Classic Translate * Rotate * Scale. (The renderer doesn't use this, see UpdateWorldCache.)
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position, 0.0f));
	transform = glm::rotate(transform, m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	return glm::scale(transform, glm::vec3(m_Scale, 1.0f));
}

void Drawable::SetPosition(const glm::vec2& position)
{
	if (position == m_Position)
	{
		return;
	}

	m_Position = position;
	m_TransformDirty = true;
}

void Drawable::SetScale(const glm::vec2& scale)
{
	if (scale == m_Scale)
	{
		return;
	}

	m_Scale = scale;
	m_TransformDirty = true;
}

void Drawable::SetRotation(float radians)
{
	if (radians == m_Rotation)
	{
		return;
	}

	m_Rotation = radians;
	m_TransformDirty = true;
}

const std::array<glm::vec3, 4>& Drawable::GetWorldVertices() const
{
	if (m_TransformDirty)
	{
		UpdateWorldCache();
	}

	return m_WorldVertices;
}

const NoctalEngine::AABB2D& Drawable::GetBounds() const
{
	if (m_TransformDirty)
	{
		UpdateWorldCache();
	}

	return m_Bounds;
}

void Drawable::UpdateWorldCache() const
{
	// Instead of building a full 4x4 matrix and multiplying each corner by it, do the 2D
	// maths directly: scale, then rotate (using one sin/cos), then translate.
	// This is the same result as GetTransform() * corner, for a fraction of the work.
	const float c = cos(m_Rotation);
	const float s = sin(m_Rotation);

	const ShapeData& shape = GetShape();

	for (size_t i = 0; i < 4; i++)
	{
		const glm::vec2 scaled = shape.Positions[i] * m_Scale;
		const glm::vec2 rotated = { scaled.x * c - scaled.y * s, scaled.x * s + scaled.y * c };
		m_WorldVertices[i] = glm::vec3(rotated + m_Position, 0.0f);
	}

	m_Bounds = NoctalEngine::AABB2D::FromPoints(m_WorldVertices);
	m_TransformDirty = false;
}
