#include <NoctalEngine.h>

class SandboxLayer : public NoctalEngine::Layer
{
public:
	SandboxLayer() : Layer("SandboxLayer")
	{

	}

	void OnUpdate() override
	{
		NE_WARN("SandboxLayer::Update");
	}
	void OnEvent(NoctalEngine::Event& event) override
	{
		NE_WARN("SandboxLayer::Event");
	}
};

class Sandbox : public NoctalEngine::Application
{
public:
	Sandbox()
	{
		//PushLayer(new SandboxLayer());
	}
	~Sandbox()
	{

	}
};

NoctalEngine::Application* NoctalEngine::CreateApplication()
{
	return new Sandbox();
}
