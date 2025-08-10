#pragma once

#ifdef NE_PLATFORM_WINDOWS

extern NoctalEngine::Application* NoctalEngine::CreateApplication();

int main(int argc, char** argv)
{
	NoctalEngine::Logger::InitLoggers();

	auto app = NoctalEngine::CreateApplication();
	NE_ENGINE_ASSERT(app, "Application creation failed (EntryPoint)");
	app->Run();

	delete app;
}

#endif // DEBUG