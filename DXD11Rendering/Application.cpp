#include "Application.h"
#include <sstream>
#include <iomanip>

Application::Application() : m_window(640, 480, "3D Renderer")
{

}

int Application::Run()
{
	while (true)
	{
		// processes all pending messages, checks if error
		if (const auto errorCode = Window::ProcessMessages()) return *errorCode;

		/// --- Main Loop ---
		// Update
		DoFrame();

		// Draw

		// Render polygons
	}
}

void Application::DoFrame()
{
	const float c = sin(m_timer.PeekDelta()) / 2.0f + 0.5f;
	m_window.getRenderer().ClearBuffer(c, c, 1.0f);
	m_window.getRenderer().DrawTestTriangle();
	m_window.getRenderer().EndFrame();
}