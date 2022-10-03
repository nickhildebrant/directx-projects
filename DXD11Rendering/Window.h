#pragma once
#include <Windows.h>
#include <optional>
#include <memory>
#include "ExceptionHandler.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Renderer.h"

class Window {
public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string title);
	static std::optional<int> ProcessMessages();

	// Accessors
	int getWidth();
	int getHeight();
	HWND getHandle();
	Renderer& getRenderer();

	Keyboard keyboard;
	Mouse mouse;

	class Exception : public ExceptionHandler {
		using ExceptionHandler::ExceptionHandler;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
	};

	class WindowHrException : public Exception {
	public:
		WindowHrException(int line, const char* file, HRESULT hresult) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;

	private:
		HRESULT m_hresult;
	};

	class MissingGraphicsException : public Exception {
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;
	};

private:
	int m_width, m_height;
	HWND m_handle;
	std::unique_ptr<Renderer> m_renderer;

	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// Singleton for window management
	class WindowClass {
	public:
		static const char* GetName();
		static HINSTANCE GetInstance();

	private:
		WindowClass();
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "Direct3D Rendering Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
};

// macro to send exceptions
#define HWND_EXCEPT(m_hresult) Window::WindowHrException(__LINE__, __FILE__, m_hresult)
#define HWND_LAST_EXCEPT() Window::WindowHrException(__LINE__, __FILE__, GetLastError())
#define HWND_NOGFX_EXCEPT() Window::Exception(__LINE__, __FILE__)