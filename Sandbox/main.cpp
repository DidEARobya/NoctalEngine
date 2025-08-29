#pragma once
#include <NoctalEngine.h>
#include "src/Sandbox2D.h"

class Sandbox : public NoctalEngine::Application
{
public:
	Sandbox() : NoctalEngine::Application()
	{
		m_GameLayer = new Sandbox2D();
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
