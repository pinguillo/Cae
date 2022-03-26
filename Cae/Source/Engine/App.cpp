#include "App.h"
#include "WindowsInclude.h"

Cae::App::App() {
	Cae::Log::Init();
	C_ENGINE_WARN("Engine logging initialized.");
	C_INFO("Client logging initialized.");

	engine = new Engine();
}

Cae::App::~App() {

}

void Cae::App::Run() {
	while (true)
	{

	}
}
