#pragma once
#include "Window.h"
#include "Timer.h"

class Application {
public:
	int Run();

	Application();

private:
	Window m_window;
	Timer m_timer;

	void DoFrame();
};