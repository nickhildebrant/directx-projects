#pragma once
#include <Windows.h>

class Window {
public:
	Window( int width, int height, const char* title );
	~Window();

	int getWidth();
	int getHeight();
	HWND getHandle();

	//Keyboard keyboard;
	//Mouse mouse;

private:
	int m_width, m_height;
	HWND m_handle;
};