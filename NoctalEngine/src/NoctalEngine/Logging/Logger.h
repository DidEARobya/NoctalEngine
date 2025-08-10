#pragma once
#include "NoctalEngine/Core.h"
#include "spdlog/spdlog.h"

namespace NoctalEngine
{
	class NOCTAL_ENGINE_API Logger
	{
	public:
		static void InitLoggers();

		inline static std::shared_ptr<spdlog::logger>& GetEngineLogger();
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger();

	private:
		static std::shared_ptr<spdlog::logger> s_EngineLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define NE_ENGINE_TRACE(...) ::NoctalEngine::Logger::GetEngineLogger()->trace(__VA_ARGS__)
#define NE_ENGINE_INFO(...) ::NoctalEngine::Logger::GetEngineLogger()->info(__VA_ARGS__)
#define NE_ENGINE_WARN(...) ::NoctalEngine::Logger::GetEngineLogger()->warn(__VA_ARGS__)
#define NE_ENGINE_ERROR(...) ::NoctalEngine::Logger::GetEngineLogger()->error(__VA_ARGS__)
#define NE_ENGINE_FATAL(...) ::NoctalEngine::Logger::GetEngineLogger()->critical(__VA_ARGS__)

#define NE_TRACE(...) ::NoctalEngine::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define NE_INFO(...) ::NoctalEngine::Logger::GetClientLogger()->info(__VA_ARGS__)
#define NE_WARN(...) ::NoctalEngine::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define NE_ERROR(...) ::NoctalEngine::Logger::GetClientLogger()->error(__VA_ARGS__)
#define NE_FATAL(...) ::NoctalEngine::Logger::GetClientLogger()->critical(__VA_ARGS__)