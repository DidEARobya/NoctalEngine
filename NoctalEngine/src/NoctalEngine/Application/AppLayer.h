#pragma once
#include "NoctalEngine.h"
#include "NoctalEngine/Input/Layer.h"
#include "NoctalEngine/Rendering/Camera/OrthographicCameraController.h"
#include <glm/glm.hpp>

namespace NoctalEngine
{
	class Event;

	class AppLayer : public Layer
	{
	public:
		AppLayer(float left, float right, float top, float bottom) : Layer("AppLayer"), m_CameraController(1280.0f / 720.0f)
		{

		}

		virtual ~AppLayer() override {};

		virtual void OnAttach() override {};
		virtual void OnDetach() override {};
		virtual void OnUpdate(float deltaTime) override { m_CameraController.Update(deltaTime); };
		virtual void OnEvent(Event& event) override { m_CameraController.OnEvent(event); };

		const glm::mat4& GetCameraViewProjection() const { return m_CameraController.GetViewProjectionMatrix(); }
	
	protected:
		NoctalEngine::OrthographicCameraController m_CameraController;
	};
}