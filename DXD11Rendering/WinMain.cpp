#include <Windows.h>
#include "Window.h"

int WINAPI WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
	Window window(640, 480);

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

	return 0;
}