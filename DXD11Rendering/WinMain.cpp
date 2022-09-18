#include <Windows.h>
#include "Window.h"

int WINAPI WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
	try // error handling
	{
		Window window(640, 480, "3D Renderer");

		MSG msg = { 0 };
		while (true)
		{
			// Catching message from window
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);	// Translates the msg
				DispatchMessage(&msg);	// Sends msg to WinProc

				// Quit and shutdown window
				if (msg.message == WM_QUIT) break;
			}

			/// --- Main Loop ---
			// Update

			// Draw

			// Render polygons
		}

		return msg.wParam;
	}
	catch (const ExceptionHandler& e) { MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONERROR); }
	catch (const std::exception& e) { MessageBox(nullptr, e.what(), "STD Exception", MB_OK | MB_ICONERROR); }
	catch (...) { MessageBox(nullptr, "No info available", "Unknown Error", MB_OK | MB_ICONERROR); }
	return 0;
}