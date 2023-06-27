#pragma once
#include "Window.h"
#include "Timer.h"

class Application {
public:
	Application();
	~Application();

	int Run();

private:
	Window m_window;
	Timer m_timer;

	std::vector<std::unique_ptr<class Box>> m_boxes;

	void DoFrame();
};