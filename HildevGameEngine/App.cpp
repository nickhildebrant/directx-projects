#include "App.h"

#include "ImGUI/imgui.h"
#include "GDIPlusManager.h"

GDIPlusManager gdipm;

App::App() : window(640, 480, "Hildev Game Engine")
{
	window.GetRenderer().SetProjection( DirectX::XMMatrixPerspectiveRH( 1.0f, 3.0f / 4.0f, 0.5f, 45.0f ) );
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

	window.GetRenderer().EndFrame();
}

void App::SpawnSimulationWindow()
{
	if ( ImGui::Begin( "Simulation Speed" ) )
	{
		ImGui::SliderFloat( "Simulation Speed", &simulationSpeed, 0.0f, 5.0f );
		ImGui::Text( "Application average %.2f ms/frame (%.0f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
		ImGui::Text( "(Hold Space to Pause) Status: %s", window.keyboard.isKeyPressed( VK_SPACE ) ? "PAUSED" : "RUNNING" );
	}

	ImGui::End();
}