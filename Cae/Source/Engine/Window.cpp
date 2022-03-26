#include "Window.h"


namespace Cae {
	Window::Window(int width, int height) : w_Width(width), w_Height(height), hInst(GetModuleHandle(nullptr)) {
        WNDCLASS wc = { };
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = WndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInst;
        wc.hIcon = nullptr;
        wc.hCursor = nullptr;
        wc.hbrBackground = nullptr;
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = g_WindowClassName;

        RegisterClass(&wc);

        RECT windowRect = { 0, 0, w_Width, w_Height };
        AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

        m_hWnd = CreateWindowEx(
            0,
            g_WindowClassName,
            L"Cae",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,       // Position
            windowRect.right - windowRect.left, // Width
            windowRect.bottom - windowRect.top, // Height
            NULL,
            NULL,
            hInst,
            NULL
        );

        if (m_hWnd == NULL) {
            
        }

        ShowWindow(m_hWnd, SW_SHOW);
	}
	Window::~Window() {

	}

	LRESULT Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
        if (msg == WM_NCCREATE) {
            const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
            Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        }
        // if we get a message before the WM_NCCREATE message, handle with default handler
        return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}