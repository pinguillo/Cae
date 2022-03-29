#pragma once
#include "WindowsInclude.h"
#include "Window.h"
#include <string>

namespace Cae {
	class Engine {
	public:
		Engine(int width, int height, const char* name);
		~Engine() {}

		int Run();
	private:
		Window* e_Window;
	};
}