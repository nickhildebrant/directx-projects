#include "Window.h"
#include "WindowsErrorMacros.h"
#include "ImGUI/imgui_impl_win32.h"
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
	if ( ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam ) ) return true;

	switch (msg)
	{
	// ***************************** Keyboard *************************************** //
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN: // sys key is for things like the ALT key
		if ( ImGui::GetIO().WantCaptureKeyboard ) break; // Ignores UI/UX Intended Input

		// lParam's 30th bit is set to 1 when held down, 0 if up before message
		// check that bit or check if autorepeat is set to true
		if (!(lParam & 0x40000000) || keyboard.GetAutorepeatStatus())
		{ keyboard.OnKeyPressed(static_cast<unsigned char>(wParam)); }
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP: // sys key is for things like the ALT key
		if ( ImGui::GetIO().WantCaptureKeyboard ) break; // Ignores UI/UX Intended Input

		keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;

	case WM_CHAR:
		if ( ImGui::GetIO().WantCaptureKeyboard ) break; // Ignores UI/UX Intended Input

		keyboard.OnChar(static_cast<unsigned char>(wParam));
		break;

	case WM_KILLFOCUS:
		keyboard.ClearState();
		break;
	// ****************************************************************************** //

	// ******************************* Mouse **************************************** //
	case WM_MOUSEMOVE:
	{
		// cursorless exclusive gets first dibs
		if ( !cursorEnabled )
		{
			if ( !mouse.IsInWindow() )
			{
				SetCapture( hWnd );
				mouse.OnMouseEnter();
				ShowCursor();
			}
			break;
		}

		if ( ImGui::GetIO().WantCaptureMouse ) break; // Ignores UI/UX Intended Input

		const POINTS pt = MAKEPOINTS(lParam);
		// if the cursor is in the window client
		if (pt.x >= 0 && pt.x < m_width && pt.y >= 0 && pt.y < m_height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				// capture and log mouse position
				SetCapture(m_handle);
				mouse.OnMouseEnter();
			}
		}
		else // if the cursor is outside the window client
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON)) mouse.OnMouseMove(pt.x, pt.y);
			else
			{
				// Release the mouse from the window when it leaves
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}

	case WM_LBUTTONDOWN:
	{
		SetForegroundWindow( m_handle );
		if ( !cursorEnabled )
		{
			ConfineCursor();
			HideCursor();
		}

		if ( ImGui::GetIO().WantCaptureMouse ) break; // Ignores UI/UX Intended Input

		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}

	case WM_LBUTTONUP:
	{
		if ( ImGui::GetIO().WantCaptureMouse ) break; // Ignores UI/UX Intended Input

		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}

	case WM_RBUTTONDOWN:
	{
		if ( ImGui::GetIO().WantCaptureMouse ) break; // Ignores UI/UX Intended Input

		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}

	case WM_RBUTTONUP:
	{
		if ( ImGui::GetIO().WantCaptureMouse ) break; // Ignores UI/UX Intended Input

		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}

	case WM_MBUTTONDOWN:
	{
		if ( ImGui::GetIO().WantCaptureMouse ) break; // Ignores UI/UX Intended Input

		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnMiddlePressed(pt.x, pt.y);
		break;
	}

	case WM_MBUTTONUP:
	{
		if ( ImGui::GetIO().WantCaptureMouse ) break; // Ignores UI/UX Intended Input

		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnMiddleReleased(pt.x, pt.y);
		break;
	}

	case WM_MOUSEWHEEL:
	{
		if ( ImGui::GetIO().WantCaptureMouse ) break; // Ignores UI/UX Intended Input

		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}

	case WM_ACTIVATE:
		// confine/free cursor on window to foreground/background if cursor disabled
		if ( !cursorEnabled )
		{
			if ( wParam & WA_ACTIVE )
			{
				ConfineCursor();
				HideCursor();
			}
			else
			{
				FreeCursor();
				ShowCursor();
			}
		}
		break;
	// ****************************************************************************** //

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

Window::Window(int width, int height, const char* name) : m_width(width), m_height(height)
{
	// Creating rect with size but not location
	RECT rect = { 0, 0, m_width, m_height };
	if(!AdjustWindowRect(&rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_VISIBLE, FALSE))	// Adjusts size for screen, sharpens image
	{
		throw HWND_LAST_EXCEPT();
	}

	// Create the window
	m_handle = CreateWindow(WindowClass::GetName(), name,										// name and id
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_VISIBLE,	// window style
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,			// size and location
		nullptr, nullptr, WindowClass::GetInstance(), this);									// parent window, menu, application handle, for multiple windows

	// check for error creating window
	if (m_handle == nullptr) throw HWND_LAST_EXCEPT();

	// intialize the GUI
	ImGui_ImplWin32_Init( m_handle );

	ShowWindow(m_handle, SW_SHOWDEFAULT);

	// Creating Renderer
	m_renderer = std::make_unique<Renderer>(m_handle, width, height);
}

Window::~Window() { ImGui_ImplWin32_Shutdown(); DestroyWindow( m_handle ); }

void Window::SetTitle(const std::string title)
{
	if (SetWindowText(m_handle, title.c_str()) == 0) throw HWND_LAST_EXCEPT();
}

void Window::EnableCursor()
{
	cursorEnabled = true;
	ShowCursor();
	EnableImGuiMouse();
	FreeCursor();
}

void Window::DisableCursor()
{
	cursorEnabled = false;
	HideCursor();
	DisableImGuiMouse();
	ConfineCursor();
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg = { 0 };
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// Quit and shutdown window
		if (msg.message == WM_QUIT) return msg.wParam;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// return empty when not quitting
	// allowed by C++17
	return {};
}

int Window::getWidth() { return m_width; }

int Window::getHeight() { return m_height; }

HWND Window::getHandle() { return m_handle; }

Renderer& Window::getRenderer()
{ 
	if (!m_renderer) throw HWND_NOGFX_EXCEPT();

	return *m_renderer; 
}

void Window::ConfineCursor() 
{
	RECT rect;
	GetClientRect( m_handle, &rect );
	MapWindowPoints( m_handle, nullptr, reinterpret_cast<POINT*>( &rect ), 2 );
	ClipCursor( &rect );
}

void Window::FreeCursor() 
{
	ClipCursor( nullptr );
}

void Window::HideCursor() 
{
	while ( ::ShowCursor( FALSE ) >= 0 );
}

void Window::ShowCursor() 
{
	while ( ::ShowCursor( TRUE ) < 0 );
}

void Window::EnableImGuiMouse() 
{
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void Window::DisableImGuiMouse() 
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}

// *** Window Exception Handling *****************************************************************
Window::WindowHrException::WindowHrException(int line, const char* file, HRESULT hresult) noexcept
	: Exception(line, file), m_hresult(hresult)
{

}

const char* Window::WindowHrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "Error Code: (" << GetErrorCode() << ")" << std::endl
		<< "Desc: " << GetErrorDescription() << std::endl
		<< GetOriginString();

	_buffer = oss.str();
	return _buffer.c_str();
}

const char* Window::WindowHrException::GetType() const noexcept { return "3DRender Window Exception"; }

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

HRESULT Window::WindowHrException::GetErrorCode() const noexcept { return m_hresult; }

std::string Window::WindowHrException::GetErrorDescription() const noexcept { return TranslateErrorCode(m_hresult); }

const char* Window::MissingGraphicsException::GetType() const noexcept { return "Graphics Window Exception [No Graphics]"; }