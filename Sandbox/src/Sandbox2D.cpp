#include "Sandbox2D.h"
#include <NoctalEngine/EntryPoint.h>
#include "glm/gtc/type_ptr.hpp"

Sandbox2D::Sandbox2D() : NoctalEngine::AppLayer(-1.6f, 1.6f, -0.9f, 0.9f), m_Colour({ glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) })
{
	glm::vec2 scale(0.1f);
	glm::vec4 colour(0.2f, 0.8f, 0.1f, 1.0f);
	m_Colour = colour;

	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			glm::vec3 position = glm::vec3(-1.5f, -1.0f, -1.0f) + glm::vec3(x * 0.105f, y * 0.105f, 0.0f);
			std::shared_ptr<Drawable> shape = NoctalEngine::Renderer::Instance().CreateDrawable(NoctalEngine::Geometry::TEX_SQUARE);
			shape->SetPosition(position);
			shape->SetScale(scale);
			shape->GetMaterial()->SetColour(colour);

			m_Squares.push_back(shape);
		}
	}

	m_CheckerBoard = NoctalEngine::Renderer::Instance().CreateTexture("Textures/Checkerboard.png");
	m_ChernoLogo = NoctalEngine::Renderer::Instance().CreateTexture("Textures/ChernoLogo.png");

	m_Shape = NoctalEngine::Renderer::Instance().CreateDrawable(NoctalEngine::Geometry::TEX_SQUARE);
	m_Shape->SetPosition(m_Shape->GetPosition() + glm::vec2(0.1f, 0.0f));
	m_Shape->SetScale(glm::vec2(1.0f));
	m_Shape->GetMaterial()->BindTexture(m_CheckerBoard);
	m_Shape->GetMaterial()->SetColour({ 1.0f, 1.0f, 1.0f, 1.0f });

	m_Shape2 = NoctalEngine::Renderer::Instance().CreateDrawable(NoctalEngine::Geometry::TEX_SQUARE);
	m_Shape2->SetPosition(m_Shape2->GetPosition() + glm::vec2(0.1f, 0.0f));
	m_Shape2->SetScale(glm::vec2(1.0f));
	m_Shape2->GetMaterial()->BindTexture(m_ChernoLogo);
	m_Shape2->GetMaterial()->SetColour({ 1.0f, 1.0f, 1.0f, 1.0f });

	m_Shape3 = NoctalEngine::Renderer::Instance().CreateDrawable(NoctalEngine::Geometry::TEX_SQUARE);
	m_Shape3->SetPosition(m_Shape->GetPosition() + glm::vec2(1.5f, 0.0f));
	m_Shape3->SetScale(glm::vec2(1.0f));
	m_Shape3->GetMaterial()->BindTexture(m_CheckerBoard);
	m_Shape3->GetMaterial()->SetColour({ 0.0f, 1.0f, 1.0f, 0.5f });

	m_Shape4 = NoctalEngine::Renderer::Instance().CreateDrawable(NoctalEngine::Geometry::TEX_SQUARE);
	m_Shape4->SetPosition(m_Shape2->GetPosition() + glm::vec2(1.5f, 0.0f));
	m_Shape4->SetScale(glm::vec2(1.0f));
	m_Shape4->GetMaterial()->BindTexture(m_ChernoLogo);
	m_Shape4->GetMaterial()->SetColour({ 1.0f, 1.0f, 1.0f, 1.5f });
}

Sandbox2D::~Sandbox2D()
{
}

void Sandbox2D::OnUpdate(float deltaTime)
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Tile Map", glm::value_ptr(m_Colour));
	ImGui::End();

	if (m_Squares.front()->GetMaterial()->GetColour() != m_Colour)
	{
		for (auto& square : m_Squares)
		{
			square->GetMaterial()->SetColour(m_Colour);
		}
	}

	glm::vec2 m_ShapePosition = m_Shape->GetPosition();

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

	m_Shape->SetPosition(m_ShapePosition);
	m_Shape2->SetPosition(m_ShapePosition);

	m_Shape3->SetPosition(m_Shape->GetPosition() + glm::vec2(1.5f, 0.0f));
	m_Shape4->SetPosition(m_Shape2->GetPosition() + glm::vec2(1.5f, 0.0f));

	NoctalEngine::AppLayer::OnUpdate(deltaTime);
}

void Sandbox2D::OnEvent(NoctalEngine::Event& event)
{
	NoctalEngine::AppLayer::OnEvent(event);
}
