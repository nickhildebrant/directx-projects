#pragma once
#include <Windows.h>
#include <optional>
#include <memory>

#include "Renderer.h"
#include "Keyboard.h"
#include "Mouse.h"

class Window {
public:
	Window( int width, int height, const char* title );
	~Window();

	int getWidth();
	int getHeight();
	HWND getHandle();
	Renderer& getRenderer();
	
	static std::optional<int> ProcessMessages();

	Keyboard keyboard;
	Mouse mouse;

private:
	int m_width, m_height;
	HWND m_handle;

	std::unique_ptr<Renderer> m_renderer;

	static LRESULT WINAPI HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	static LRESULT WINAPI HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	LRESULT HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	// Singleton for window management
	class WindowClass {
	public:
		static const char* GetName();
		static HINSTANCE GetInstance();

	private:
		WindowClass();
		~WindowClass();
		WindowClass( const WindowClass& ) = delete;
		WindowClass& operator=( const WindowClass& ) = delete;
		static constexpr const char* wndClassName = "My Game Engine";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
};