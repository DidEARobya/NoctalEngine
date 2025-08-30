#pragma once
#include <NoctalEngine.h>

class Sandbox2D : public NoctalEngine::AppLayer
{
public:
	Sandbox2D();
	~Sandbox2D() override;

	virtual void OnUpdate(float deltaTime) override;
	virtual void OnEvent(NoctalEngine::Event& event) override;

private:
	std::shared_ptr<Drawable> m_Shape;
	std::shared_ptr<Drawable> m_Shape2;
	std::shared_ptr<Drawable> m_Shape3;
	std::shared_ptr<Drawable> m_Shape4;

	std::vector<std::shared_ptr<Drawable>> m_Squares;
	glm::vec4 m_Colour;

	std::shared_ptr<NoctalEngine::Texture> m_CheckerBoard;
	std::shared_ptr<NoctalEngine::Texture> m_ChernoLogo;
};