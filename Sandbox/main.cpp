#pragma once
#include <NoctalEngine.h>

class Sandbox : public NoctalEngine::Application
{
public:
	Sandbox() : NoctalEngine::Application()
	{
		NoctalEngine::Renderer::Instance().CreateDrawable(NoctalEngine::Geometry::SQUARE);
		NoctalEngine::Renderer::Instance().CreateDrawable(NoctalEngine::Geometry::TRIANGLE);
	}
	~Sandbox()
	{

	}
};

NoctalEngine::Application* NoctalEngine::CreateApplication()
{
	return new Sandbox();
}
