#pragma once
#include "WindowsInclude.h"
#include "Window.h"
#include "Graphics.h"
#include <string>

namespace Cae {
	class Engine {
	public:
		Engine(int width, int height, const char* name);
		~Engine() {}

		int Run();
	private:
		void HandleInputs();

		Window* e_Window;
		Graphics* e_Graphics;
	};
}