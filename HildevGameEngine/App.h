#pragma once
#include "Window.h"
#include "Camera.h"
#include "PointLight.h"
#include "Timer.h"

class App {
public:
	App();
	~App();

	int Run();

private:
	Window window;
	Timer timer;

	Camera camera;
	Light light;

	void RenderFrame();
};