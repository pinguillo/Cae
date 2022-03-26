#pragma once
#include "Window.h"

namespace Cae {
	class Engine {
	public:
		Engine();
		~Engine();

		void InitWindow();

	private:
		Window* e_Window;
	};
}