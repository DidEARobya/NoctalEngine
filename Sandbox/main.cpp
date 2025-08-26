#pragma once
#include <NoctalEngine.h>
#include "NoctalEngine/Application/AppLayer.h"

class AppLayer : public NoctalEngine::AppLayer
{
public:
	AppLayer() : NoctalEngine::AppLayer(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_CameraSpeed(2.0f)
	{
		//m_Square = NoctalEngine::Renderer::Instance().CreateDrawable(NoctalEngine::Geometry::SQUARE);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int i = 0; i < 5; i++)
		{
			for (int u = 0; u < 5; u++)
			{
				glm::vec3 position(i * 0.22f, u * 0.22f, 0.0f);
				std::shared_ptr<Drawable> shape = NoctalEngine::Renderer::Instance().CreateDrawable(NoctalEngine::Geometry::SQUARE);
				shape->SetPosition(position);
				shape->SetScale(scale);
			}
		}

		m_Triangle = NoctalEngine::Renderer::Instance().CreateDrawable(NoctalEngine::Geometry::TRIANGLE);
	}

	~AppLayer() override
	{
		m_Triangle.reset();
		//m_Square.reset();
	}

	void OnUpdate(float deltaTime) override
	{
		if (NoctalEngine::InputManager::IsKeyDown(KEY_LEFT))
		{
			m_CameraPosition.x -= m_CameraSpeed * NoctalEngine::Application::DeltaTime();
		}

		if (NoctalEngine::InputManager::IsKeyDown(KEY_RIGHT))
		{
			m_CameraPosition.x += m_CameraSpeed * NoctalEngine::Application::DeltaTime();
		}

		if (NoctalEngine::InputManager::IsKeyDown(KEY_UP))
		{
			m_CameraPosition.y += m_CameraSpeed * NoctalEngine::Application::DeltaTime();
		}

		if (NoctalEngine::InputManager::IsKeyDown(KEY_DOWN))
		{
			m_CameraPosition.y -= m_CameraSpeed * NoctalEngine::Application::DeltaTime();
		}

		glm::vec3 m_TrianglePosition = m_Triangle->GetPosition();

		if (NoctalEngine::InputManager::IsKeyDown(KEY_A))
		{
			m_TrianglePosition.x -= m_CameraSpeed * NoctalEngine::Application::DeltaTime();
		}

		if (NoctalEngine::InputManager::IsKeyDown(KEY_D))
		{
			m_TrianglePosition.x += m_CameraSpeed * NoctalEngine::Application::DeltaTime();
		}

		if (NoctalEngine::InputManager::IsKeyDown(KEY_W))
		{
			m_TrianglePosition.y += m_CameraSpeed * NoctalEngine::Application::DeltaTime();
		}

		if (NoctalEngine::InputManager::IsKeyDown(KEY_S))
		{
			m_TrianglePosition.y -= m_CameraSpeed * NoctalEngine::Application::DeltaTime();
		}

		m_Triangle->SetPosition(m_TrianglePosition);
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

	std::shared_ptr<Drawable> m_Triangle;
	//std::shared_ptr<Drawable> m_Square;
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
