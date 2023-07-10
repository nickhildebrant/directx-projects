#include "Window.h"
#include <sstream>

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() : hInstance( GetModuleHandle( nullptr ) )
{
	// Defining the window style
	WNDCLASS windowClass = { 0 };
	windowClass.style = CS_OWNDC;
	windowClass.lpfnWndProc = HandleMsgSetup;
	windowClass.hInstance = GetInstance();
	windowClass.hIcon = nullptr;
	windowClass.hCursor = LoadCursor( nullptr, IDC_ARROW );
	windowClass.hbrBackground = nullptr;
	windowClass.lpszClassName = GetName();
	RegisterClass( &windowClass );
}

Window::WindowClass::~WindowClass() { UnregisterClass( wndClassName, GetInstance() ); }

const char* Window::WindowClass::GetName() { return wndClassName; }

HINSTANCE Window::WindowClass::GetInstance() { return wndClass.hInstance; }

LRESULT CALLBACK WinProc( HWND handle, UINT msg, WPARAM wparam, LPARAM lparam )
{
	if ( msg == WM_DESTROY || msg == WM_CLOSE )
	{
		PostQuitMessage( 0 );
		return 0;
	}

	switch ( msg )
	{
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
		break;

	case WM_CLOSE:
		PostQuitMessage( 0 );
		return 0;
		break;

	}

	return DefWindowProc( handle, msg, wparam, lparam );
}

Window::Window( int width, int height, const char* title ) : m_width(width), m_height(height)
{
	// Defining the size and type of the window
	RECT rect = { 0, 0, m_width, m_height };
	AdjustWindowRect( &rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_VISIBLE, FALSE ); // Adjusts size for screen, sharpens image

	// Create the window
	m_handle = CreateWindow( "My Game Engine", title,									// name and id
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_VISIBLE,	// window style
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,			// size and location, (0, 0)
		nullptr, nullptr, nullptr, nullptr );													// parent window, menu, application handle, for multiple windows

	ShowWindow( m_handle, SW_SHOWDEFAULT );
}

Window::~Window() { DestroyWindow( m_handle ); }

int Window::getWidth() { return m_width; }
int Window::getHeight() { return m_height; }

HWND Window::getHandle() { return m_handle; }

// *** Message Handling **************************************************************************
LRESULT WINAPI Window::HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if ( msg == WM_NCCREATE )
	{
		// extract pointer to window class
		const CREATESTRUCTW* const ptrCreate = reinterpret_cast<CREATESTRUCTW*>( lParam );
		Window* const ptrWindow = static_cast<Window*>( ptrCreate->lpCreateParams );

		// setting the WinAPI user data to window class
		SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( ptrWindow ) );

		// setting the message proc to normal, after setup
		SetWindowLongPtr( hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( &Window::HandleMsgThunk ) );

		return ptrWindow->HandleMsg( hWnd, msg, wParam, lParam );
	}

	// if a message is receieved before the create message, its handled by the default handler
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

LRESULT WINAPI Window::HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	// exrtact pointer to window class
	Window* const ptrWindow = reinterpret_cast<Window*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
	return ptrWindow->HandleMsg( hWnd, msg, wParam, lParam );
}

LRESULT Window::HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
		// ***************************** Keyboard *************************************** //
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN: // sys key is for things like the ALT key
		// lParam's 30th bit is set to 1 when held down, 0 if up before message
		// check that bit or check if autorepeat is set to true
		if ( !( lParam & 0x40000000 ) || keyboard.GetAutorepeatStatus() )
		{
			keyboard.OnKeyPressed( static_cast<unsigned char>( wParam ) );
		}
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP: // sys key is for things like the ALT key
		keyboard.OnKeyReleased( static_cast<unsigned char>( wParam ) );
		break;

	case WM_CHAR:
		keyboard.OnChar( static_cast<unsigned char>( wParam ) );
		break;

	case WM_KILLFOCUS:
		keyboard.ClearState();
		break;
		// ****************************************************************************** //

		// ******************************* Mouse **************************************** //
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS( lParam );
		// if the cursor is in the window client
		if ( pt.x >= 0 && pt.x < m_width && pt.y >= 0 && pt.y < m_height )
		{
			mouse.OnMouseMove( pt.x, pt.y );
			if ( !mouse.IsInWindow() )
			{
				// capture and log mouse position
				SetCapture( m_handle );
				mouse.OnMouseEnter();
			}
		}
		else // if the cursor is outside the window client
		{
			if ( wParam & ( MK_LBUTTON | MK_RBUTTON ) ) mouse.OnMouseMove( pt.x, pt.y );
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
		const POINTS pt = MAKEPOINTS( lParam );
		mouse.OnLeftPressed( pt.x, pt.y );
		break;
	}

	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS( lParam );
		mouse.OnLeftReleased( pt.x, pt.y );
		break;
	}

	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS( lParam );
		mouse.OnRightPressed( pt.x, pt.y );
		break;
	}

	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS( lParam );
		mouse.OnRightReleased( pt.x, pt.y );
		break;
	}

	case WM_MBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS( lParam );
		mouse.OnMiddlePressed( pt.x, pt.y );
		break;
	}

	case WM_MBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS( lParam );
		mouse.OnMiddleReleased( pt.x, pt.y );
		break;
	}

	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS( lParam );
		const int delta = GET_WHEEL_DELTA_WPARAM( wParam );
		mouse.OnWheelDelta( pt.x, pt.y, delta );
		break;
	}
	// ****************************************************************************** //

	case WM_CLOSE:
		PostQuitMessage( 0 );
		return 0;
		break;

	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
		break;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}