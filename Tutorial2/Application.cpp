#include "Application.h"
#include <memory>
#include <algorithm>

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
	m_window.getRenderer().ClearBuffer(0.07f, 0.0f, 0.12f, 1.0f);	// sets background color
	
	float deltaTime = m_timer.DeltaTime();
	for (auto& model : m_models)
	{
		model->Update( m_window.keyboard.isKeyPressed( VK_SPACE ) ? 0.0f : deltaTime );
		model->Draw( m_window.getRenderer() );
	}

	m_window.getRenderer().EndFrame();
}