#pragma once
#include <Windows.h>

#include "Keyboard.h"
#include "Mouse.h"

class Window {
public:
	Window( int width, int height, const char* title );
	~Window();

	int getWidth();
	int getHeight();
	HWND getHandle();

	Keyboard keyboard;
	Mouse mouse;

private:
	int m_width, m_height;
	HWND m_handle;

	static LRESULT WINAPI HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	static LRESULT WINAPI HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	LRESULT HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
};