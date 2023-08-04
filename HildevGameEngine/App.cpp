#include "App.h"

App::App() : window(640, 480, "Hildev Game Engine")
{
	window.GetRenderer().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 3.0f / 4.0f, 0.5f, 45.0f ) );
}

App::~App()
{

}

int App::Run()
{
	while ( true )
	{
		// processes all pending messages, checks if error
		if ( const auto errorCode = Window::ProcessMessages() ) return *errorCode;

		window.GetRenderer().SetCameraView( camera.GetMatrix() );
		//light.Bind( window.GetRenderer(), camera.GetMatrix() );

		/// --- Main Loop ---
		RenderFrame();
	}
}

void App::RenderFrame()
{
	showUI = window.keyboard.isKeyPressed( VK_ESCAPE ) ? false : true;

	window.GetRenderer().BeginFrame( 0.07f, 0.0f, 0.12f, 1.0f );	// sets background color

	float deltaTime = simulationSpeed * timer.DeltaTime();

	//light.Draw( window.GetRenderer() );

	// Sim speed UI
	//SpawnSimulationWindow();

	// imgui windows for lights and camera
	camera.SpawnControlWindow();
	//light.SpawnControlWindow();

	// Box UI
	//SpawnBoxManagerWindow();
	//SpawnBoxWindows();

	window.GetRenderer().EndFrame();
}