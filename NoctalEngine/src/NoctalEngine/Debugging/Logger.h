#pragma once
#include "NoctalEngine/Core.h"
#include "spdlog/spdlog.h"
#include "imgui.h"

namespace NoctalEngine
{
	class Logger
	{
	public:
		static void InitLoggers();

		inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return s_EngineLogger; };
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; };

		inline static const char* Basename(const char* path)
		{
			const char* file = strrchr(path, '/');
			if (!file) file = strrchr(path, '\\');
			return file ? file + 1 : path;
		}

	private:
		static std::shared_ptr<spdlog::logger> s_EngineLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define __FILENAME__ ::NoctalEngine::Logger::Basename(__FILE__)

#define NE_ENGINE_TRACE(...) ::NoctalEngine::Logger::GetEngineLogger()->trace(__VA_ARGS__)
#define NE_ENGINE_INFO(...) ::NoctalEngine::Logger::GetEngineLogger()->info(__VA_ARGS__)
#define NE_ENGINE_WARN(fmt, ...) ::NoctalEngine::Logger::GetEngineLogger()->warn( "[{} (Line {})] - " fmt, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define NE_ENGINE_ERROR(fmt, ...) ::NoctalEngine::Logger::GetEngineLogger()->error( "[{} (Line {})] - " fmt, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define NE_ENGINE_FATAL(fmt, ...) ::NoctalEngine::Logger::GetEngineLogger()->critical( "[{} (Line {})] - " fmt, __FILENAME__, __LINE__, ##__VA_ARGS__))

#define NE_TRACE(...) ::NoctalEngine::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define NE_INFO(...) ::NoctalEngine::Logger::GetClientLogger()->info(__VA_ARGS__)
#define NE_WARN(fmt, ...) ::NoctalEngine::Logger::GetEngineLogger()->warn( "[{} (Line {})] - " fmt, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define NE_ERROR(fmt, ...) ::NoctalEngine::Logger::GetEngineLogger()->error( "[{} (Line {})] - " fmt, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define NE_FATAL(fmt, ...) ::NoctalEngine::Logger::GetEngineLogger()->critical( "[{} (Line {})] - " fmt, __FILENAME__, __LINE__, ##__VA_ARGS__))