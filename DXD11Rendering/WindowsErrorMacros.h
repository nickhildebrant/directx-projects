#pragma once

// macro to send exceptions
#define HWND_EXCEPT(m_hresult) Window::WindowHrException(__LINE__, __FILE__, m_hresult)
#define HWND_LAST_EXCEPT() Window::WindowHrException(__LINE__, __FILE__, GetLastError())
#define HWND_NOGFX_EXCEPT() Window::Exception(__LINE__, __FILE__)