#include "Window.h"

namespace Cae {
	Window::Window(int width, int height, const char* name) : hInst(GetModuleHandle(nullptr)) {
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

        if (!RegisterClassEx(&wndClass)) { MessageBox(NULL, L"xd", L"xd", MB_OK); }

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

        if(!w_HWnd) { MessageBoxW(NULL, (LPCWSTR)GetLastError(), L"xd", MB_OK); }

        ShowWindow(w_HWnd, SW_SHOW);
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
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}