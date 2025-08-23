#pragma once
#include "NoctalEngine/Core.h"
#include "spdlog/spdlog.h"
#include "imgui.h"

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
#define NE_ENGINE_WARN(...) ::NoctalEngine::Logger::GetEngineLogger()->warn("{0} (Line {1}): {2}", __FILE__, __LINE__, __VA_ARGS__)
#define NE_ENGINE_ERROR(...) ::NoctalEngine::Logger::GetEngineLogger()->error("{0} (Line {1}): {2}", __FILE__, __LINE__, __VA_ARGS__)
#define NE_ENGINE_FATAL(...) ::NoctalEngine::Logger::GetEngineLogger()->critical("{0} (Line {1}): {2}", __FILE__, __LINE__, __VA_ARGS__)

#define NE_TRACE(...) ::NoctalEngine::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define NE_INFO(...) ::NoctalEngine::Logger::GetClientLogger()->info(__VA_ARGS__)
#define NE_WARN(...) ::NoctalEngine::Logger::GetClientLogger()->warn("{0} (Line {1}): {2}", __FILE__, __LINE__, __VA_ARGS__)
#define NE_ERROR(...) ::NoctalEngine::Logger::GetClientLogger()->error("{0} (Line {1}): {2}", __FILE__, __LINE__, __VA_ARGS__)
#define NE_FATAL(...) ::NoctalEngine::Logger::GetClientLogger()->critical("{0} (Line {1}): {2}", __FILE__, __LINE__, __VA_ARGS__)