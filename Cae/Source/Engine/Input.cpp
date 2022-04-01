#include "Input.h"
#include "Log.h"

bool Cae::Input::GetKey(WPARAM key) {
	if (GetKeyState(key) & 0x8000) {
		return true;
	}
	return false;
}

void Cae::Input::GetKeyPress(WPARAM wParam) {
	//C_ENGINE_INFO( );
}
