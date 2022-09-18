#pragma once
#include <Windows.h>

class Window {
public:
	Window(int width, int height);
	~Window();
	int getWidth();
	int getHeight();
	HWND getHandle();

private:
	int m_width, m_height;
	HWND m_handle;
};