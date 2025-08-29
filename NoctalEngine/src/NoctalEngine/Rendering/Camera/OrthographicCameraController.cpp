#include "OrthographicCameraController.h"
#include "NoctalEngine/Input/InputManager.h"
#include "NoctalEngine/Input/KeyCodes.h"
#include "NoctalEngine/Events/Event.h"
#include "NoctalEngine/Events/WindowEvents.h"
#include "NoctalEngine/Events/MouseEvents.h"
#include "glm/gtc/matrix_transform.hpp"

namespace NoctalEngine
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio) : m_AspectRatio(aspectRatio), 
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{
	}
	void OrthographicCameraController::Update(float deltaTime)
	{
		m_CameraTranslationSpeed = m_ZoomLevel;

		if (InputManager::IsKeyDown(KEY_LEFT))
		{
			m_CameraPosition.x -= m_CameraTranslationSpeed * deltaTime;
		}

		if (InputManager::IsKeyDown(KEY_RIGHT))
		{
			m_CameraPosition.x += m_CameraTranslationSpeed * deltaTime;
		}

		if (InputManager::IsKeyDown(KEY_UP))
		{
			m_CameraPosition.y += m_CameraTranslationSpeed * deltaTime;
		}

		if (InputManager::IsKeyDown(KEY_DOWN))
		{
			m_CameraPosition.y -= m_CameraTranslationSpeed * deltaTime;
		}

		CalculateViewMatrix();
	}

	void OrthographicCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrollEvent>(NOCTAL_BIND_EVENT_FN(OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(NOCTAL_BIND_EVENT_FN(OnWindowResize));
	}

	bool OrthographicCameraController::OnMouseScrolled(const MouseScrollEvent& event)
	{
		m_ZoomLevel -= event.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

		m_Camera.m_ProjectionMatrix = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -1.0f, 1.0f);
		m_Camera.m_ViewProjectionMatrix = m_Camera.m_ProjectionMatrix * m_Camera.m_ViewMatrix;

		return false;
	}
	bool OrthographicCameraController::OnWindowResize(const WindowResizeEvent& event)
	{
		m_AspectRatio -= event.GetWidth() / event.GetHeight();

		m_Camera.m_ProjectionMatrix = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -1.0f, 1.0f);
		m_Camera.m_ViewProjectionMatrix = m_Camera.m_ProjectionMatrix * m_Camera.m_ViewMatrix;

		return false;
	}

	void OrthographicCameraController::CalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_CameraPosition);

		m_Camera.m_ViewMatrix = glm::inverse(transform);
		m_Camera.m_ViewProjectionMatrix = m_Camera.m_ProjectionMatrix * m_Camera.m_ViewMatrix;
	}
}