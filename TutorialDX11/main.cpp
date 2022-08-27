#include <Windows.h>
#include "Window.h"
#include "Renderer.h"
#include "Triangle.h"

int WINAPI WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
	Window window(640, 640);
	Renderer renderer(window);
	Triangle triangle(renderer);

	MSG msg = { 0 };
	while (true)
	{
		// Catching message from window
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			// Quit and shutdown window
			if (msg.message == WM_QUIT) break;
		}

		/// --- Main Loop ---
		// Update
		
		// Draw
		renderer.beginFrame();
		// Render polygons
		triangle.draw(renderer);
		renderer.endFrame();
	}

	return 0;
}