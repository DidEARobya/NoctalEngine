#include <NoctalEngine.h>


class Sandbox : public NoctalEngine::Application
{
public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}
};

NoctalEngine::Application* NoctalEngine::CreateApplication()
{
	return new Sandbox();
}
