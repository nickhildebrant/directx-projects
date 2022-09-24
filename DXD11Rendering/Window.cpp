#include "Window.h"
#include <sstream>

// *** Windows Class *********************************************************************
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass()
	: hInst(GetModuleHandle(nullptr))
{
	WNDCLASS wc = { 0 };
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = nullptr;
	wc.lpszClassName = GetName();
	RegisterClass(&wc);
}

Window::WindowClass::~WindowClass() { UnregisterClass(wndClassName, GetInstance()); }

const char* Window::WindowClass::GetName() { return wndClassName; }

HINSTANCE Window::WindowClass::GetInstance() { return wndClass.hInst; }

// *** Message Handling **************************************************************************
LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		// extract pointer to window class
		const CREATESTRUCTW* const ptrCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const ptrWindow = static_cast<Window*>(ptrCreate->lpCreateParams);

		// setting the WinAPI user data to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(ptrWindow));

		// setting the message proc to normal, after setup
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

		return ptrWindow->HandleMsg(hWnd, msg, wParam, lParam);
	}

	// if a message is receieved before the create message, its handled by the default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// exrtact pointer to window class
	Window* const ptrWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return ptrWindow->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		break;

	case WM_KEYUP:
		keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;

	case WM_CHAR:
		keyboard.OnChar(static_cast<unsigned char>(wParam));
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI WinProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
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
	}

	return DefWindowProc(handle, msg, wparam, lparam);
}

Window::Window(int width, int height, const char* name)
{
	m_width = width; m_height = height;

	// Creating rect with size but not location
	RECT rect = { 0, 0, width, height };
	AdjustWindowRect(&rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_VISIBLE, FALSE);	// Adjusts size for screen, sharpens image

	// Create the window
	m_handle = CreateWindow(WindowClass::GetName(), name,										// name and id
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_VISIBLE,	// window style
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,			// size and location
		nullptr, nullptr, WindowClass::GetInstance(), this);									// parent window, menu, application handle, for multiple windows

	// check for error creating window
	if (m_handle == nullptr) throw HWND_LAST_EXCEPT();

	ShowWindow(m_handle, SW_SHOWDEFAULT);
}

Window::~Window() { DestroyWindow(m_handle); }

int Window::getWidth() { return m_width; }

int Window::getHeight() { return m_height; }

HWND Window::getHandle() { return m_handle; }

// *** Window Exception Handling *****************************************************************
Window::Exception::Exception(int line, const char* file, HRESULT hresult) noexcept
	: ExceptionHandler(line, file), m_hresult(hresult)
{

}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "Error Code: (" << GetErrorCode() << ")" << std::endl
		<< "Desc: " << GetErrorString() << std::endl
		<< GetOriginString();

	_buffer = oss.str();
	return _buffer.c_str();
}

const char* Window::Exception::GetType() const noexcept { return "3DRender Window Exception"; }

std::string Window::Exception::TranslateErrorCode(HRESULT hresult) noexcept
{
	char* ptrMsgBuffer = nullptr;
	DWORD nMsgLength = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hresult,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&ptrMsgBuffer),
		0, nullptr);

	if (nMsgLength == 0) return "Unidentified error code";

	std::string errorString = ptrMsgBuffer;
	LocalFree(ptrMsgBuffer);
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept { return m_hresult; }

std::string Window::Exception::GetErrorString() const noexcept { return TranslateErrorCode(m_hresult); }