#include "Cae.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "wb", stdout);

	Cae::Log::Init();
	C_INFO("Client logger started.");
		
	Cae::Engine* engine = new Cae::Engine(1280, 720, "Cae Engine");
	
	int e = engine->Run();
	delete engine;
	return e;
}