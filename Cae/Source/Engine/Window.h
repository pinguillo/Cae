#pragma once
#include "WindowsInclude.h"
#include "Log.h"

namespace Cae {
	class Window {
	public:
		Window(int width, int height, const char* name);
		~Window() {}

		static int ProcessMessages();
	private:
		static LRESULT CALLBACK HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		HWND w_HWnd;
		static constexpr const char* w_ClassName = "Cae Window";
		HINSTANCE hInst;
	};
}