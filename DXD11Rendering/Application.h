#pragma once
#include "Window.h"
#include "Timer.h"

class Application {
public:
	Application();

	int Run();

private:
	Window m_window;
	Timer m_timer;

	void DoFrame();
};