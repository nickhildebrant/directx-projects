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
	m_width = width; m_height = height;

	// Define window style
	WNDCLASS wc = { 0 };
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WinProc;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = "DX11Tutorial";
	RegisterClass(&wc);

	// Creating rect with size but not location
	RECT rect = { 0, 0, width, height };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);	// Adjusts size for screen, sharpens image

	// Create the window
	m_handle = CreateWindow("DX11Tutorial", "DX11 Tutorial",		// name and id
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,							// window style
		0, 0, rect.right - rect.left, rect.bottom - rect.top,		// size and location
		nullptr, nullptr, nullptr, nullptr);						// parent window, menu, application handle, for multiple windows
}

int Window::getWidth() { return m_width; }

int Window::getHeight() { return m_height; }

HWND Window::getHandle() { return m_handle; }