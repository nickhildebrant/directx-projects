#include "Application.h"
#include <sstream>
#include <iomanip>

Application::Application() : window(640, 480, "3D Renderer")
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
	
}