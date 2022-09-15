#include "Window.h"
#include <sstream>

LRESULT CALLBACK WinProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;

	case WM_KEYDOWN:
		if (wparam == 'W') SetWindowText(handle, "W is pressed");
		if (wparam == 'S') SetWindowText(handle, "S is pressed");
		break;

	case WM_CHAR:
		break;

	case WM_LBUTTONDOWN:
		const POINTS pt = MAKEPOINTS(lparam);
		std::ostringstream oss;
		oss << "(" << pt.x << "," << pt.y << ")";
		SetWindowText(handle, oss.str().c_str());
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
	wc.lpszClassName = "3DRenderer";
	RegisterClass(&wc);

	// Creating rect with size but not location
	RECT rect = { 0, 0, width, height };
	AdjustWindowRect(&rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_VISIBLE, FALSE);	// Adjusts size for screen, sharpens image

	// Create the window
	m_handle = CreateWindow("3DRenderer", "3D Renderer",									// name and id
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_VISIBLE,  // window style
		0, 0, rect.right - rect.left, rect.bottom - rect.top,									// size and location
		nullptr, nullptr, nullptr, nullptr);													// parent window, menu, application handle, for multiple windows
}

int Window::getWidth() { return m_width; }

int Window::getHeight() { return m_height; }

HWND Window::getHandle() { return m_handle; }