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
		DoFrame();
	}
}

void Application::DoFrame()
{
	const float c = sin(m_timer.PeekDelta()) / 2.0f + 0.5f; // changes color on background, based on sin wave
	m_window.getRenderer().ClearBuffer(c, c, 1.0f, 1.0f);	// sets background color
	m_window.getRenderer().DrawTestTriangle(
												m_timer.PeekDelta(), 
												m_window.mouse.GetMouseX() / ((float) m_window.getWidth() / 2.0f) - 1.0f,
												-m_window.mouse.GetMouseY() / ((float)m_window.getHeight() / 2.0f) + 1.0f
											); // Draws triangle
	m_window.getRenderer().EndFrame();
}