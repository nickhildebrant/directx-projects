#include "Window.h"

LRESULT CALLBACK WinProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_DESTROY || msg == WM_CLOSE)
	{
		PostQuitMessage(0);
		return 0;
	}

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
	m_handle = CreateWindow("DX11Tutorial", "DX11 Tutorial",					// name and id
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,		// window style
		500, 100, width, height,												// size and location
		nullptr, nullptr, nullptr, nullptr);									// parent window, menu, application handle, for multiple windows
}

HWND Window::getHandle()
{
	return m_handle;
}