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
	m_window.DisableCursor();
	m_window.getRenderer().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 45.0f));
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

		/// --- Main Loop ---
		RenderFrame();
	}
}

void Application::RenderFrame()
{
	showUI = m_window.keyboard.isKeyPressed( VK_ESCAPE ) ? false : true;

	m_window.getRenderer().BeginFrame( 0.07f, 0.0f, 0.12f, 1.0f );	// sets background color
	
	float deltaTime = simulationSpeed * m_timer.DeltaTime();

	nano.Draw( m_window.getRenderer() );
	light.Draw( m_window.getRenderer() );

	// imgui windows for lights and camera
	camera.SpawnControlWindow();
	light.SpawnControlWindow();
	//ShowDemoUI();
	nano.ShowWindow();

	m_window.getRenderer().EndFrame();
}

void Application::ShowDemoUI()
{
	static bool show_demo_window = true;
	if ( show_demo_window )
	{
		ImGui::ShowDemoWindow( &show_demo_window );
	}

	ImGui::End();
}