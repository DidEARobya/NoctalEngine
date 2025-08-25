#pragma once
#include <NoctalEngine.h>
#include "NoctalEngine/Application/AppLayer.h"

class AppLayer : public NoctalEngine::AppLayer
{
public:
	AppLayer() : NoctalEngine::AppLayer(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_CameraSpeed(2.0f)
	{
		NoctalEngine::Renderer::Instance().CreateDrawable(NoctalEngine::Geometry::SQUARE);
		NoctalEngine::Renderer::Instance().CreateDrawable(NoctalEngine::Geometry::TRIANGLE);
	}

	void OnUpdate(float deltaTime) override
	{
		if (NoctalEngine::InputManager::IsKeyDown(KEY_LEFT))
		{
			m_CameraPosition.x += m_CameraSpeed * NoctalEngine::Application::DeltaTime();
		}

		if (NoctalEngine::InputManager::IsKeyDown(KEY_RIGHT))
		{
			m_CameraPosition.x -= m_CameraSpeed * NoctalEngine::Application::DeltaTime();
		}

		if (NoctalEngine::InputManager::IsKeyDown(KEY_UP))
		{
			m_CameraPosition.y -= m_CameraSpeed * NoctalEngine::Application::DeltaTime();
		}

		if (NoctalEngine::InputManager::IsKeyDown(KEY_DOWN))
		{
			m_CameraPosition.y += m_CameraSpeed * NoctalEngine::Application::DeltaTime();
		}

		m_Camera.SetPosition(m_CameraPosition);
	}

	void OnEvent(NoctalEngine::Event& event) override
	{
		//NoctalEngine::EventDispatcher dispatcher(event);
		//dispatcher.Dispatch<NoctalEngine::KeyPressedEvent>(NOCTAL_BIND_EVENT_FN(AppLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(const NoctalEngine::KeyPressedEvent& event)
	{
		return false;
	}

private:
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed;
};

class Sandbox : public NoctalEngine::Application
{
public:
	Sandbox() : NoctalEngine::Application()
	{
		m_GameLayer = new AppLayer();
		PushLayer(m_GameLayer);
	}
	~Sandbox()
	{
	}

	virtual const glm::mat4& GetCameraViewProjection() const override { return m_GameLayer->GetCameraViewProjection(); };
};

NoctalEngine::Application* NoctalEngine::CreateApplication()
{
	return new Sandbox();
}
