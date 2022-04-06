#pragma once
#include <string>
#include <chrono>
#include "Engine\Log.h"

namespace Cae {
	std::wstring ToWide(const std::string& narrow) {
		wchar_t wide[512];
		mbstowcs_s(nullptr, wide, narrow.c_str(), _TRUNCATE);
		return wide;
	}

	class ScopedTimer {
	public:
		ScopedTimer(const char* name) : m_Name(name), m_Timer(false) {
			m_Start = std::chrono::steady_clock::now();
		}
		~ScopedTimer() {
			if (!m_Timer) {
				auto endTime = std::chrono::steady_clock::now();
				auto startTime = std::chrono::duration<double, std::micro>{ m_Start.time_since_epoch() };
				auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_Start).time_since_epoch();
			
				C_ENGINE_TRACE("Time spent in '{0}': {1} ms.", m_Name, elapsedTime.count());

				m_Timer = true;
			}
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
		bool m_Timer;
	};
}