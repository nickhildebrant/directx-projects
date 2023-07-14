#include <Windows.h>

#include "Window.h"
#include "Renderer.h"
#include "Triangle.h"
#include "Box.h"

#include <random>

int WINAPI WinMain( HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount )
{
	Window window( 640, 480, "My Game Engine" );
	Renderer renderer( window );
	//Triangle triangle( renderer );
	Box box( renderer );

	MSG msg = { 0 };
	while ( true )
	{
		// Catching message from window
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );	// Translates the msg
			DispatchMessage( &msg );	// Sends msg to WinProc

			// Quit and shutdown window
			if ( msg.message == WM_QUIT ) break;
		}

		// Draw
		renderer.BeginFrame();

		// Render polygons
		//triangle.draw( renderer );

		renderer.EndFrame();
	}

	return 0;
}