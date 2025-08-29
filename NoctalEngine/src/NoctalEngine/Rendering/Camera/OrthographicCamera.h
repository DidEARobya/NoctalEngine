#pragma once
#include <glm/glm.hpp>

namespace NoctalEngine
{
	class OrthographicCameraController;

	struct OrthographicCamera
	{
	private:
		friend class OrthographicCameraController;
		OrthographicCamera(float left, float right, float bottom, float top);

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
	};
}