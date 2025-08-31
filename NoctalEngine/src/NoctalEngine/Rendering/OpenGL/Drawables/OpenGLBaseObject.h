#pragma once
//#include "OpenGLDrawableBase.h"
#include "OpenGLDrawable.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "NoctalEngine/Rendering/Geometry/Geometry.h"

//To implement later
//template <class T>
//class OpenGLBaseObject : public OpenGLDrawableBase<T>

//template <class T>
class OpenGLBaseObject : public OpenGLDrawable
{
public:
	OpenGLBaseObject(glm::vec2 position = glm::vec2(0.0f), glm::vec2 scale = glm::vec2(1.0f)) : m_Position(glm::vec3(position, 0.0f)), m_Scale(glm::scale(glm::mat4(1.0f), {scale.x , scale.y, 1.0f}))
	{
	}
	virtual glm::mat4 GetTransform() const override
	{
		return glm::translate(glm::mat4(1.0f), m_Position) * m_Scale;
	}

	virtual void SetPosition(const glm::vec2& position) override { m_Position = glm::vec3(position, 0.0f); }
	virtual glm::vec2 GetPosition() override { return m_Position; }

	virtual void SetScale(const glm::vec2& scale) override { m_Scale = glm::scale(glm::mat4(1.0f), { scale.x , scale.y, 1.0f }); }

	struct Vertex
	{
		glm::vec3 Pos;
		glm::vec2 TexCoord;
	};

protected:
	glm::vec3 m_Position;
	glm::mat4 m_Scale;
};