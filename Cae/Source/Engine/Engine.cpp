#include "Engine.h"

namespace Cae {
	Engine::Engine() {
		Engine::InitWindow();
	}
	void Engine::InitWindow() {
		e_Window = new Window(1280, 720);
	}
}