#include <Windows.h>

#include "Window.h"
#include "Renderer.h"
#include "Timer.h"
#include "Box.h"

#include <random>

int WINAPI WinMain( HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount )
{
	Window window( 640, 480, "My Game Engine" );
	Renderer renderer( window );
	Timer timer;

    // Random cube
	const float PI = 3.14159265f;
	std::mt19937 rng( std::random_device{}( ) );
	std::uniform_real_distribution<float> adist( 0.0f, PI * 2.0f );
	std::uniform_real_distribution<float> ddist( 0.0f, PI * 2.0f );
	std::uniform_real_distribution<float> odist( 0.0f, PI * 0.3f );
	std::uniform_real_distribution<float> rdist( 6.0f, 20.0f );
	std::uniform_real_distribution<float> bdist( 0.4f, 3.0f );
	Box box( renderer, rng, adist, ddist, odist, rdist, bdist );

	renderer.SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 3.0f / 4.0f, 0.5f, 45.0f ) );

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

		float deltaTime = timer.DeltaTime();

		// Draw
		renderer.BeginFrame();

		// Render polygons
		box.Update( deltaTime );
		box.Draw( renderer );
		renderer.EndFrame();
	}

	return 0;
}