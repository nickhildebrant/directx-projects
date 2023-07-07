#include "Window.h"

LRESULT CALLBACK WinProc( HWND handle, UINT msg, WPARAM wparam, LPARAM lparam )
{
	if ( msg == WM_DESTROY || msg == WM_CLOSE )
	{
		PostQuitMessage( 0 );
		return 0;
	}

	return DefWindowProc( handle, msg, wparam, lparam );
}

Window::Window( int width, int height, const char* title ) : m_width(width), m_height(height)
{
	// Defining the window style
	WNDCLASS windowClass = { 0 };
	windowClass.style = CS_OWNDC;
	windowClass.lpfnWndProc = WinProc;
	windowClass.hCursor = LoadCursor( nullptr, IDC_ARROW );
	windowClass.lpszClassName = L"My Game Engine";
	RegisterClass( &windowClass );

	// Defining the size and type of the window
	RECT rect = { 0, 0, m_width, m_width };
	AdjustWindowRect( &rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_VISIBLE, FALSE ); // Adjusts size for screen, sharpens image

	// Create the window
	m_handle = CreateWindow( L"My Game Engine", (LPCWSTR)title,									// name and id
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_VISIBLE,	// window style
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,			// size and location, (0, 0)
		nullptr, nullptr, nullptr, nullptr );													// parent window, menu, application handle, for multiple windows

	ShowWindow( m_handle, SW_SHOWDEFAULT );
}

Window::~Window()
{

}

int Window::getWidth() { return m_width; }
int Window::getHeight() { return m_height; }

HWND Window::getHandle() { return m_handle; }