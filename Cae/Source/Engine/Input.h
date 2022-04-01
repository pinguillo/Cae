#pragma once
#include "WindowsInclude.h"

namespace Cae {
	class Input {
	public:
		Input() = default;

		static bool GetKey(WPARAM key);
		
		static void GetKeyPress(WPARAM wParam); // TODO: Pls help me god, how do I do an event system or whatever I need for this
	private:
	};
}