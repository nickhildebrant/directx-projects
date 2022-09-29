#pragma once
#include "Window.h"
#include "Timer.h"

class Application {
public:
	int Run();

	Application();

private:
	Window window;
	Timer timer;

	void DoFrame();
};