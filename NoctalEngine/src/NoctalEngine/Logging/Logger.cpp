#include "nepch.h"
#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace NoctalEngine
{
	std::shared_ptr<spdlog::logger> Logger::s_EngineLogger = spdlog::stdout_color_mt("NoctalEngine");
	std::shared_ptr<spdlog::logger> Logger::s_ClientLogger = spdlog::stdout_color_mt("App");

	void Logger::InitLoggers()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		
		assert(s_EngineLogger, "Engine Logger isn't initialised");
		s_EngineLogger->set_level(spdlog::level::trace);
		assert(s_EngineLogger, "Client Logger isn't initialised");
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
