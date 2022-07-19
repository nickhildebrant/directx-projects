#include <Windows.h>
#include "Window.h"

LRESULT CALLBACK WinProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(handle, msg, wparam, lparam);
}

Window::Window(int width, int height)
{
	// Define window style
	WNDCLASS wc = { 0 };
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WinProc;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = "DX11Tutorial";
	RegisterClass(&wc);

	// Create the window
	CreateWindow("DX11Tutorial", "DX11 Tutorial",
		WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
		100, 100, width, height,
		nullptr, nullptr, nullptr, nullptr);
}