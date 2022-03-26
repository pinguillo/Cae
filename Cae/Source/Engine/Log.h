#pragma once
#include <memory>
#include "spdlog\spdlog.h"

namespace Cae {
	class Log {
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return l_EngineLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return l_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> l_EngineLogger;
		static std::shared_ptr<spdlog::logger> l_ClientLogger;
	};
}

// Engine macros
#define C_ENGINE_TRACE(...)		::Cae::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define C_ENGINE_INFO(...)		::Cae::Log::GetEngineLogger()->info(__VA_ARGS__)
#define C_ENGINE_WARN(...)		::Cae::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define C_ENGINE_ERROR(...)		::Cae::Log::GetEngineLogger()->error(__VA_ARGS__)

// Client macros
#define C_TRACE(...)		::Cae::Log::GetClientLogger()->trace(__VA_ARGS__)
#define C_INFO(...)		::Cae::Log::GetClientLogger()->info(__VA_ARGS__)
#define C_WARN(...)		::Cae::Log::GetClientLogger()->warn(__VA_ARGS__)
#define C_ERROR(...)		::Cae::Log::GetClientLogger()->error(__VA_ARGS__)