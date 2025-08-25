#pragma once
#include "NoctalEngine.h"
#include "NoctalEngine/Input/Layer.h"
#include <glm/glm.hpp>

namespace NoctalEngine
{
	class AppLayer : public Layer
	{
	public:
		AppLayer(float left, float right, float top, float bottom) : Layer("AppLayer"), m_Camera(left, right, top, bottom)
		{

		}

		virtual ~AppLayer() override {};

		virtual void OnAttach() override {};
		virtual void OnDetach() override {};
		virtual void OnUpdate(float deltaTime) override {};

		const glm::mat4& GetCameraViewProjection() const { return m_Camera.GetViewProjectionMatrix(); }
	
	protected:
		NoctalEngine::OrthographicCamera m_Camera;
	};
}