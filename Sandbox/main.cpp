#include <NoctalEngine.h>


class Sandbox : public NoctalEngine::Application
{
public:
	Sandbox()
	{
		PushOverlay(new NoctalEngine::ImGuiLayer());
	}
	~Sandbox()
	{

	}
};

NoctalEngine::Application* NoctalEngine::CreateApplication()
{
	return new Sandbox();
}
