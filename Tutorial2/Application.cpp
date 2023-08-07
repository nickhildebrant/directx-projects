#include "Application.h"
#include <memory>
#include <algorithm>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_impl_dx11.h"

#include "Surface.h"
#include "GDIPlusManager.h"

GDIPlusManager gdipm;

Application::Application() : m_window(800, 600, "3D Renderer"), light(m_window.getRenderer())
{
	const size_t numberOfModels = 180;
	ModelFactory factory(m_window.getRenderer());
	m_models.reserve(numberOfModels);

	std::generate_n(std::back_inserter(m_models), numberOfModels, factory);

	m_window.getRenderer().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 45.0f));

	// init box pointers for editing instance parameters
	for ( auto& pDrawable : m_models )
	{
		if ( auto pBox = dynamic_cast<Box*>( pDrawable.get() ) )
		{
			boxes.push_back( pBox );
		}
	}
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
	for (auto& model : m_models)
	{
		model->Update( m_window.keyboard.isKeyPressed( VK_SPACE ) ? 0.0f : deltaTime );
		model->Draw( m_window.getRenderer() );
	}

	light.Draw( m_window.getRenderer() );

	// Sim speed UI
	SpawnSimulationWindow();

	// imgui windows for lights and camera
	camera.SpawnControlWindow();
	light.SpawnControlWindow();

	// Box UI
	SpawnBoxManagerWindow();
	SpawnBoxWindows();

	m_window.getRenderer().EndFrame();
}

void Application::SpawnSimulationWindow()
{
	//if ( showUI ) ImGui::ShowDemoWindow( &showUI ); // Demo UI
	if ( ImGui::Begin( "Simulation Speed" ) )
	{
		ImGui::SliderFloat( "Simulation Speed", &simulationSpeed, 0.0f, 5.0f );
		ImGui::Text( "Application average %.2f ms/frame (%.0f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
		ImGui::Text( "(Hold Space to Pause) Status: %s", m_window.keyboard.isKeyPressed( VK_SPACE ) ? "PAUSED" : "RUNNING" );
	}

	ImGui::End();
}

void Application::SpawnBoxManagerWindow()
{
	// imgui window to open box windows
	if ( ImGui::Begin( "Boxes" ) )
	{
		using namespace std::string_literals;
		const auto preview = comboBoxIndex ? std::to_string( *comboBoxIndex ) : "Choose a box..."s;
		if ( ImGui::BeginCombo( "Box Number", preview.c_str() ) )
		{
			for ( int i = 0; i < boxes.size(); i++ )
			{
				const bool selected = true;
				if ( ImGui::Selectable( std::to_string( i ).c_str(), selected ) )
				{
					comboBoxIndex = i;
				}

				if ( selected ) { ImGui::SetItemDefaultFocus(); }
			}

			ImGui::EndCombo();
		}

		if ( ImGui::Button( "Spawn Control Window" ) && comboBoxIndex )
		{
			boxIDs.insert( *comboBoxIndex );
			comboBoxIndex.reset();
		}
	}

	ImGui::End();
}

void Application::SpawnBoxWindows()
{
	// imgui box attribute control windows
	for ( auto i = boxIDs.begin(); i != boxIDs.end(); )
	{
		if ( !boxes[*i]->SpawnControlWindow( m_window.getRenderer(), *i ) )
		{
			i = boxIDs.erase( i );
		}
		else
		{
			i++;
		}
	}
}