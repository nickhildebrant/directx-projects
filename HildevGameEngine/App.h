#pragma once
#include "GUIManager.h"
#include "Window.h"
#include "Camera.h"
#include "PointLight.h"
#include "Timer.h"

class App {
public:
	App();
	~App();

	int Run();

	void SpawnSimulationWindow();

private:
	GUIManager guiManager;

	Window window;
	Timer timer;

	Camera camera;
	//Light light;

	bool showUI = true;
	float simulationSpeed = 1.0f;

	void RenderFrame();
};