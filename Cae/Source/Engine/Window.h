#pragma once
#include "WindowsInclude.h"
#include "Input.h"
#include "Graphics.h"

namespace Cae {
	class Window {
	public:
		Window(int width, int height, const char* name);
		~Window() {}

		HWND GethWnd() { return w_HWnd; }

		int GetWidth() { return w_Width; }
		int GetHeight() { return w_Height; }

		Graphics* GetGraphics() { return w_Graphics; }

		static int ProcessMessages();
	private:
		static LRESULT CALLBACK HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		HWND w_HWnd;
		static constexpr const char* w_ClassName = "Cae Window";
		HINSTANCE hInst;

		Graphics* w_Graphics;
	private:
		int w_Width;
		int w_Height;
	public:
		 static Input w_Input;
	};
}