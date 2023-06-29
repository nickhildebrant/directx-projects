#include "Application.h"
#include <sstream>
#include <iomanip>

#include "Box.h"
#include <memory>

Application::Application() : m_window(640, 480, "3D Renderer")
{
	// Random cubes
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 1.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.08f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 180; i++)
	{
		m_boxes.push_back(std::make_unique<Box>(m_window.getRenderer(), rng, adist, ddist, odist, rdist));
	}
	m_window.getRenderer().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.1f, 100.0f));
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
	for (auto& box : m_boxes)
	{
		box->Update(deltaTime);
		box->Draw(m_window.getRenderer());
	}

	m_window.getRenderer().EndFrame();
}