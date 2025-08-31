#include "Sandbox2D.h"
#include <NoctalEngine/EntryPoint.h>
#include "glm/gtc/type_ptr.hpp"
#include "NoctalEngine/Debugging/ImGuiLayer.h"
#include "NoctalEngine/Window/Window.h"

Sandbox2D::Sandbox2D() : NoctalEngine::AppLayer(-1.6f, 1.6f, -0.9f, 0.9f), m_Colour({ glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) })
{
	NoctalEngine::Application::Get().GetWindow().SetVSync(true);
	m_EnableVSync = NoctalEngine::Application::Get().GetWindow().IsVSync();

	m_CheckerBoard = NoctalEngine::Renderer::Instance().CreateTexture("Textures/Checkerboard.png");
	m_ChernoLogo = NoctalEngine::Renderer::Instance().CreateTexture("Textures/ChernoLogo.png");

	glm::vec2 scale(0.1f);
	glm::vec4 colour(0.2f, 0.8f, 0.1f, 1.0f);
	m_Colour = colour;

	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			glm::vec3 position = glm::vec3(-1.5f, -1.0f, -1.0f) + glm::vec3(x * 0.105f, y * 0.105f, 0.0f);
			std::shared_ptr<Drawable> shape = NoctalEngine::Renderer::Instance().CreateDrawable(NoctalEngine::Geometry::TRIANGLE, position, scale);
			shape->GetMaterial()->SetColour(colour);

			m_Shapes.push_back(shape);
		}
	}

	m_Shape = NoctalEngine::Renderer::Instance().CreateDrawable(NoctalEngine::Geometry::QUAD, glm::vec2(0.1f, 0.0f));
	m_Shape->GetMaterial()->SetBaseTexture(m_CheckerBoard);
	m_Shape->GetMaterial()->SetColour({ 1.0f, 1.0f, 1.0f, 1.0f });

	m_Shape2 = NoctalEngine::Renderer::Instance().CreateDrawable(NoctalEngine::Geometry::QUAD, m_Shape->GetPosition());
	m_Shape2->GetMaterial()->SetBaseTexture(m_ChernoLogo);
	m_Shape2->GetMaterial()->SetColour({ 1.0f, 1.0f, 1.0f, 1.0f });

	m_Shape3 = NoctalEngine::Renderer::Instance().CreateDrawable(NoctalEngine::Geometry::QUAD, m_Shape->GetPosition() + glm::vec2(1.5f, 0.0f));
	m_Shape3->GetMaterial()->SetBaseTexture(m_CheckerBoard);
	m_Shape3->GetMaterial()->SetColour({ 0.0f, 1.0f, 1.0f, 0.5f });

	m_Shape4 = NoctalEngine::Renderer::Instance().CreateDrawable(NoctalEngine::Geometry::QUAD, m_Shape3->GetPosition());
	m_Shape4->GetMaterial()->SetBaseTexture(m_ChernoLogo);
	m_Shape4->GetMaterial()->SetColour({ 1.0f, 1.0f, 1.0f, 1.5f });
}

Sandbox2D::~Sandbox2D()
{
}

void Sandbox2D::OnUpdate(float deltaTime)
{
	NE_SCOPE_TIMER("Sandbox2D", "OnUpdate");

	if (m_Shapes.front()->GetMaterial()->GetColour() != m_Colour)
	{
		for (auto& square : m_Shapes)
		{
			square->GetMaterial()->SetColour(m_Colour);
		}
	}

	glm::vec2 direction(0.0f);

	if (NoctalEngine::InputManager::IsKeyDown(KEY_A))
	{
		direction.x -= 2.5f * NoctalEngine::Application::DeltaTime();
	}

	if (NoctalEngine::InputManager::IsKeyDown(KEY_D))
	{
		direction.x += 2.5f * NoctalEngine::Application::DeltaTime();
	}

	if (NoctalEngine::InputManager::IsKeyDown(KEY_W))
	{
		direction.y += 2.5f * NoctalEngine::Application::DeltaTime();
	}

	if (NoctalEngine::InputManager::IsKeyDown(KEY_S))
	{
		direction.y -= 2.5f * NoctalEngine::Application::DeltaTime();
	}

	m_Shape->SetPosition(m_Shape->GetPosition() + direction);
	m_Shape2->SetPosition(m_Shape2->GetPosition() + direction);

	m_Shape3->SetPosition(m_Shape->GetPosition() + glm::vec2(1.5f, 0.0f));
	m_Shape4->SetPosition(m_Shape2->GetPosition() + glm::vec2(1.5f, 0.0f));

	NoctalEngine::AppLayer::OnUpdate(deltaTime);
}

void Sandbox2D::OnEvent(NoctalEngine::Event& event)
{
	NoctalEngine::AppLayer::OnEvent(event);
}

void Sandbox2D::LateUpdate(float deltaTime)
{
	static bool vSync = m_EnableVSync;

	ImGui::Begin("Settings");
	ImGui::Checkbox("VSync", &vSync);

	ToggleVSync(vSync);

	ImGui::ColorEdit4("Tile Map", glm::value_ptr(m_Colour));
	ImGui::End();
}

void Sandbox2D::ToggleVSync(bool val)
{
	if (m_EnableVSync == val)
	{
		return;
	}

	m_EnableVSync = val;
	NoctalEngine::Application::Get().GetWindow().SetVSync(m_EnableVSync);
}
