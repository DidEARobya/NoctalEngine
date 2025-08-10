#pragma once

#ifdef NE_PLATFORM_WINDOWS

extern NoctalEngine::Application* NoctalEngine::CreateApplication();

int main(int argc, char** argv)
{
	NoctalEngine::Logger::InitLoggers();
	NE_ENGINE_WARN("Initizialised Log");
	NE_FATAL("Fatal test, Var={0}", 5);

	auto app = NoctalEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif // DEBUG