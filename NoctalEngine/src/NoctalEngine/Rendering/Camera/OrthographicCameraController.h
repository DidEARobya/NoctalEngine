#pragma once
#include "OrthographicCamera.h"
#include "glm/glm.hpp"

namespace NoctalEngine
{
	class Event;
	class MouseScrollEvent;
	class WindowResizeEvent;

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio);

		void Update(float deltaTime);
		void OnEvent(Event& e);

		const glm::mat4& GetViewProjectionMatrix() const { return m_Camera.m_ViewProjectionMatrix; };
		void SetAspectRatio(const float aspectRatio) { m_AspectRatio = aspectRatio; };

	private:
		bool OnMouseScrolled(const MouseScrollEvent& event);
		bool OnWindowResize(const WindowResizeEvent& event);

		void CalculateViewMatrix();

	private:
		float m_AspectRatio = 0.0f;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraTranslationSpeed = 2.5f;
	};
}