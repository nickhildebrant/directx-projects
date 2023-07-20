#include "Application.h"
#include <memory>
#include <algorithm>

Application::Application() : m_window( 640, 480, "My Game Engine" ), m_renderer( m_window )
{
	const size_t numberOfModels = 180;
	ModelFactory factory( m_renderer );
	m_models.reserve( numberOfModels );

	std::generate_n( std::back_inserter( m_models ), numberOfModels, factory );

	ball = new Ball( m_renderer, 1, 16, 8 );

	m_renderer.SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f, 3.0f / 4.0f, 0.5f, 45.0f ) );
}

Application::~Application() {}

int Application::Run()
{
	while ( true )
	{
		// processes all pending messages, checks if error
		if ( const auto errorCode = Window::ProcessMessages() ) return *errorCode;

		/// --- Main Loop ---
		RenderFrame();
	}
}

void Application::RenderFrame()
{
	m_renderer.BeginFrame();
	m_renderer.ClearBuffer( 0.07f, 0.0f, 0.12f, 1.0f );	// sets background color

	float deltaTime = m_timer.DeltaTime();
	for ( auto& model : m_models )
	{
		//model->Update( deltaTime );
		//model->Draw( m_renderer );
	}

	ball->Draw( m_renderer );

	m_renderer.EndFrame();
}