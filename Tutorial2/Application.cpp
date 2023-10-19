#include "Application.h"
#include <memory>
#include <algorithm>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_impl_dx11.h"

#include "Surface.h"
#include "GDIPlusManager.h"

GDIPlusManager gdipm;

Application::Application() : m_window(1280, 720, "3D Renderer"), light(m_window.getRenderer())
{
	wall.SetRootTransform( DirectX::XMMatrixTranslation( -5.0f, 0.0f, 0.0f ) );
	testPlane.SetPosition( { 5.0f,0.0f,0.0f,1.0f } );
	m_window.getRenderer().SetProjection(DirectX::XMMatrixPerspectiveRH(1.0f, 9.0f / 16.0f, 0.5f, 45.0f));
}

Application::~Application() {}

int Application::Run()
{
	while (true)
	{
		// processes all pending messages, checks if error
		if (const auto errorCode = Window::ProcessMessages()) return *errorCode;

		m_window.getRenderer().SetCameraView( camera.GetMatrix() );
		light.Bind( m_window.getRenderer(), camera.GetMatrix() );

		while ( const auto e = m_window.keyboard.ReadKey() )
		{
			if ( !e->IsPressed() )
			{
				continue;
			}
			
			switch( e->GetCode() )
			{
			case VK_ESCAPE:
				if ( m_window.IsCursorEnabled() )
				{
					m_window.DisableCursor();
					m_window.mouse.EnableRawInput();
				}
				else
				{
					m_window.EnableCursor();
					m_window.mouse.DisableRawInput();
				}
				break;

			case VK_F1:
				showDemoUI = true;
				break;
			}
		}

		/// --- Main Loop ---
		RenderFrame();
	}
}

void Application::RenderFrame()
{
	showUI = m_window.keyboard.isKeyPressed( VK_ESCAPE ) ? false : true;

	m_window.getRenderer().BeginFrame( 0.07f, 0.0f, 0.12f, 1.0f );	// sets background color
	
	float deltaTime = simulationSpeed * m_timer.DeltaTime();

	// Camera Controls
	if ( !m_window.IsCursorEnabled() )
	{
		if ( m_window.keyboard.isKeyPressed( 'W' ) ) camera.Translate( { 0.0f, 0.0f, -deltaTime, 1.0f } );
		if ( m_window.keyboard.isKeyPressed( 'A' ) ) camera.Translate( { -deltaTime, 0.0f, 0.0f, 1.0f } );
		if ( m_window.keyboard.isKeyPressed( 'S' ) ) camera.Translate( { 0.0f, 0.0f, deltaTime, 1.0f } );
		if ( m_window.keyboard.isKeyPressed( 'D' ) ) camera.Translate( { deltaTime, 0.0f, 0.0f, 1.0f } );
		if ( m_window.keyboard.isKeyPressed( 'E' ) ) camera.Translate( { 0.0f, deltaTime, 0.0f, 1.0f } );
		if ( m_window.keyboard.isKeyPressed( 'Q' ) ) camera.Translate( { 0.0f, -deltaTime, 0.0f, 1.0f } );
	}

	while ( const auto delta = m_window.mouse.ReadRawDelta() )
	{
		if ( !m_window.IsCursorEnabled() )
		{
			camera.Rotate( (float) -delta->y, (float) -delta->x, 0.0f );
		}
	}

	goblin.Draw( m_window.getRenderer() );
	wall.Draw( m_window.getRenderer() );
	testPlane.Draw( m_window.getRenderer() );
	light.Draw( m_window.getRenderer() );

	// imgui windows for lights and camera
	//camera.SpawnControlWindow();
	light.SpawnControlWindow();

	//ShowDemoUI();
	testPlane.SpawnControlWindow( m_window.getRenderer() );
	wall.ShowWindow( m_window.getRenderer(), "Wall" );
	goblin.ShowWindow( m_window.getRenderer(), "Goblin" );

	m_window.getRenderer().EndFrame();
}