#include "Engine.h"
#include "Log.h"

namespace Cae {
	Engine::Engine(int width, int height, const char* name)	{
		C_ENGINE_INFO("Engine logger started.");
		e_Window = new Window(width, height, name);
		e_Graphics = e_Window->GetGraphics();
	}

	int Engine::Run() {
		while (true) {
			if (const auto ecode = Window::ProcessMessages() != OKCODE) {
				return ecode;
			}
			HandleInputs();

			e_Graphics->RenderFrame();

		}
	}

	void Engine::HandleInputs()	{
		if (Input::GetKey(VK_SPACE)) {
			C_ENGINE_INFO("xd"); // Input system testing
		}
	}
}