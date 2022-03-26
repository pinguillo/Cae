#include "Log.h"
#include "spdlog\sinks\stdout_color_sinks.h"

namespace Cae {
	std::shared_ptr<spdlog::logger> Log::l_EngineLogger;
	std::shared_ptr<spdlog::logger> Log::l_ClientLogger;
	
	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");

		l_EngineLogger = spdlog::stdout_color_mt("Engine");
		l_EngineLogger->set_level(spdlog::level::trace);

		l_ClientLogger = spdlog::stdout_color_mt("App");
		l_ClientLogger->set_level(spdlog::level::trace);
	}

}