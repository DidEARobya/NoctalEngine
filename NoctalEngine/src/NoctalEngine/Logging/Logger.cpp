#include "nepch.h"
#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace NoctalEngine
{
	std::shared_ptr<spdlog::logger> Logger::s_EngineLogger;
	std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;

	void Logger::InitLoggers()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_EngineLogger = spdlog::stdout_color_mt("NoctalEngine");
		s_EngineLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("App");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger>& Logger::GetEngineLogger()
	{
		return s_EngineLogger;
	}
	std::shared_ptr<spdlog::logger>& Logger::GetClientLogger()
	{
		return s_ClientLogger;
	}
}
