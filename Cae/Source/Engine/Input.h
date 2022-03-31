#pragma once
#include "WindowsInclude.h"

namespace Cae {
	class Input {
	public:
		Input() = default;

		static bool GetKey(WPARAM key);
		
		static void KeyPress(WPARAM wParam);
	private:
	};
}