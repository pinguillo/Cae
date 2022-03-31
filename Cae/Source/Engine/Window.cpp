#include "Window.h"
#include "Log.h"

namespace Cae {
	Window::Window(int width, int height, const char* name) : hInst(GetModuleHandle(nullptr)), w_Width(width), w_Height(height) {
        WNDCLASSEX wndClass = { 0 };
        wndClass.cbSize = sizeof(wndClass);
        wndClass.style = CS_OWNDC;
        wndClass.lpfnWndProc = HandleMsg;
        wndClass.hInstance = hInst;
        wndClass.hCursor = nullptr;
        wndClass.hIcon = nullptr;
        wndClass.hbrBackground = nullptr;
        wndClass.lpszMenuName = nullptr;
        wndClass.lpszClassName = (LPCWSTR)w_ClassName;

        if (!RegisterClassEx(&wndClass)) { C_ENGINE_ERROR(GetLastError()); }

        RECT windowRect = { 0, 0, width, height };
        AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

        w_HWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
                                (LPCWSTR)w_ClassName,
                                (LPCWSTR)name,
                                WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                windowRect.right - windowRect.left,
                                windowRect.bottom - windowRect.top,
                                nullptr, nullptr, hInst, nullptr
        );

        if(!w_HWnd) { C_ENGINE_ERROR(GetLastError()); }

        ShowWindow(w_HWnd, SW_SHOW);

        w_Graphics = new Graphics(w_HWnd, width, height);

	}

    int Window::ProcessMessages() {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                return (int)msg.wParam;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return OKCODE;
    }

    LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
            case WM_CLOSE:
                PostQuitMessage(0);
                return 0;
            case WM_KEYDOWN:
                w_Input.KeyPress(wParam);
                break;
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}