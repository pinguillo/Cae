#pragma once
#include "WindowsInclude.h"

namespace Cae {
	class Window {
	public:
		Window(int width, int height);
		~Window();

		static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HINSTANCE hInst;
		HWND m_hWnd;
		LPCWSTR g_WindowClassName = (LPCWSTR)L"CaeWindow";
	private:
		int w_Width;
		int w_Height;
	};
}