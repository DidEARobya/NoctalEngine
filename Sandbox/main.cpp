#pragma once
#include <NoctalEngine.h>
#include "glm/gtc/type_ptr.hpp"

class AppLayer : public NoctalEngine::AppLayer
{
public:
	AppLayer() : NoctalEngine::AppLayer(-1.6f, 1.6f, -0.9f, 0.9f), m_Colour({ glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) })
	{
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		glm::vec4 colour(0.2f, 0.8f, 0.1f, 1.0f);

		for (int x = 0; x < 5; x++)
		{
			for (int y = 0; y < 5; y++)
			{
				glm::vec3 position = glm::vec3(-1.5f, -1.0f, -1.0f) + glm::vec3(x * 0.105f, y * 0.105f, 0.0f);
				std::shared_ptr<Drawable> shape = NoctalEngine::Renderer::Instance().CreateDrawable(NoctalEngine::Geometry::SQUARE);
				shape->SetPosition(position);
				shape->SetScale(scale);
				shape->GetMaterial()->Uniforms.Colour = colour;

				m_Squares.push_back(shape);
			}
		}

		m_CheckerBoard = NoctalEngine::Renderer::Instance().CreateTexture("Textures/Checkerboard.png");
		m_ChernoLogo = NoctalEngine::Renderer::Instance().CreateTexture("Textures/ChernoLogo.png");
		
		m_Square = NoctalEngine::Renderer::Instance().CreateDrawable(NoctalEngine::Geometry::TEX_SQUARE);
		m_Square->SetPosition(m_Square->GetPosition() + glm::vec3(0.3f, 0.0f, 0.0f));
		m_Square->SetScale(glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_Square->GetMaterial()->BindTexture(m_CheckerBoard);

		m_Square2 = NoctalEngine::Renderer::Instance().CreateDrawable(NoctalEngine::Geometry::TEX_SQUARE);
		m_Square2->SetPosition(m_Square2->GetPosition() + glm::vec3(0.3f, 0.0f, 0.0f));
		m_Square2->SetScale(glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_Square2->GetMaterial()->BindTexture(m_ChernoLogo);
	}

	~AppLayer() override
	{
	}

	void OnUpdate(float deltaTime) override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Tile Map", glm::value_ptr(m_Colour));
		ImGui::End();

		if (m_Squares.front()->GetMaterial()->Uniforms.Colour != m_Colour)
		{
			for (auto& square : m_Squares)
			{
				square->GetMaterial()->Uniforms.Colour = m_Colour;
			}
		}

		glm::vec3 m_ShapePosition = m_Square->GetPosition();

		if (NoctalEngine::InputManager::IsKeyDown(KEY_A))
		{
			m_ShapePosition.x -= 2.5f * NoctalEngine::Application::DeltaTime();
		}

		if (NoctalEngine::InputManager::IsKeyDown(KEY_D))
		{
			m_ShapePosition.x += 2.5f * NoctalEngine::Application::DeltaTime();
		}

		if (NoctalEngine::InputManager::IsKeyDown(KEY_W))
		{
			m_ShapePosition.y += 2.5f * NoctalEngine::Application::DeltaTime();
		}

		if (NoctalEngine::InputManager::IsKeyDown(KEY_S))
		{
			m_ShapePosition.y -= 2.5f * NoctalEngine::Application::DeltaTime();
		}

		m_Square->SetPosition(m_ShapePosition);
		m_Square2->SetPosition(m_ShapePosition);

		NoctalEngine::AppLayer::OnUpdate(deltaTime);
	}

	void OnEvent(NoctalEngine::Event& event) override
	{
		NoctalEngine::AppLayer::OnEvent(event);
	}

private:
	std::shared_ptr<Drawable> m_Square;
	std::shared_ptr<Drawable> m_Square2;

	std::vector<std::shared_ptr<Drawable>> m_Squares;
	glm::vec4 m_Colour;

	std::shared_ptr<NoctalEngine::Texture> m_CheckerBoard;
	std::shared_ptr<NoctalEngine::Texture> m_ChernoLogo;
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
