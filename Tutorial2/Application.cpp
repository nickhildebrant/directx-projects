#include "Application.h"
#include <memory>
#include <algorithm>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_impl_dx11.h"

#include "Surface.h"
#include "GDIPlusManager.h"

GDIPlusManager gdipm;

Application::Application() : m_window(640, 480, "3D Renderer")
{
	const size_t numberOfModels = 180;
	ModelFactory factory(m_window.getRenderer());
	m_models.reserve(numberOfModels);

	std::generate_n(std::back_inserter(m_models), numberOfModels, factory);

	m_window.getRenderer().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 45.0f));
	m_window.getRenderer().SetCameraView( DirectX::XMMatrixTranslation( 0.0f, 0.0f, 20.0f ) );
}

Application::~Application() {}

int Application::Run()
{
	while (true)
	{
		// processes all pending messages, checks if error
		if (const auto errorCode = Window::ProcessMessages()) return *errorCode;

		/// --- Main Loop ---
		DoFrame();
	}
}

void Application::DoFrame()
{
	showUI = m_window.keyboard.isKeyPressed( VK_ESCAPE ) ? false : true;

	m_window.getRenderer().BeginFrame( 0.07f, 0.0f, 0.12f, 1.0f );	// sets background color
	
	float deltaTime = simulationSpeed * m_timer.DeltaTime();
	for (auto& model : m_models)
	{
		model->Update( m_window.keyboard.isKeyPressed( VK_SPACE ) ? 0.0f : deltaTime );
		model->Draw( m_window.getRenderer() );
	}

	//if ( showUI ) ImGui::ShowDemoWindow( &showUI ); // Demo UI
	if ( ImGui::Begin( "Simulation Speed" ) )
	{
		ImGui::SliderFloat( "Simulation Speed", &simulationSpeed, 0.0f, 5.0f );
		ImGui::Text( "Application average %.2f ms/frame (%.0f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
		ImGui::Text( "Status: %s", m_window.keyboard.isKeyPressed( VK_SPACE ) ? "PAUSED" : "RUNNING" );
	}
	ImGui::End();

	m_window.getRenderer().EndFrame();
}