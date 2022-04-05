#pragma once
#include <string>

namespace Cae {
	std::wstring ToWide(const std::string& narrow) {
		wchar_t wide[512];
		mbstowcs_s(nullptr, wide, narrow.c_str(), _TRUNCATE);
		return wide;
	}
}