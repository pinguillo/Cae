#include "Engine.h"

namespace Cae {
	Engine::Engine(int width, int height, const char* name)	{
		C_ENGINE_INFO("Engine logger started.");
		e_Window = new Window(width, height, name);

	}

	int Engine::Run() {
		while (true) {
			if (const auto ecode = Window::ProcessMessages() != OKCODE) {
				return ecode;
			}
		}
	}
}