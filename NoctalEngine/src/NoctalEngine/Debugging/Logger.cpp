#include "nepch.h"
#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/dup_filter_sink.h"

namespace NoctalEngine
{
	using spdlog::sinks::dup_filter_sink_st;
	using spdlog::sinks::stdout_color_sink_mt;

	std::shared_ptr<spdlog::logger> Logger::s_EngineLogger;
	std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;

	void Logger::InitLoggers()
	{
		auto dup_filter = std::make_shared<dup_filter_sink_st>(std::chrono::seconds(30));
		dup_filter->add_sink(std::make_shared<stdout_color_sink_mt>());

		s_EngineLogger = std::make_shared<spdlog::logger>("NoctalEngine", dup_filter);
		assert(s_EngineLogger, "Engine Logger isn't initialised");
		s_EngineLogger->set_level(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("Client", dup_filter);
		assert(s_ClientLogger, "Client Logger isn't initialised");
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
