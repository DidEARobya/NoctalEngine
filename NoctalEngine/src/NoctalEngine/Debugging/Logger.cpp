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

		//NE_ENGINE_TRACE("NE_TEST_T (Logger)");
		//NE_ENGINE_INFO("NE_TEST_I (Logger)");
		//NE_ENGINE_WARN("NE_TEST_W");
		//NE_ENGINE_ERROR("NE_TEST_E");
		//NE_ENGINE_FATAL("NE_TEST_F");

		//NE_TRACE("TEST_T (Logger)");
		//NE_INFO("TEST_I (Logger)");
		//NE_WARN("TEST_W (Logger)");
		//NE_ERROR("TEST_E (Logger)");
		//NE_FATAL("TEST_F (Logger)");
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
