#pragma once
#include <set>

#include "GUIManager.h"
#include "Window.h"
#include "Timer.h"
#include "Model.h"

#include "Camera.h"
#include "Light.h"

#include "TestPlane.h"
#include "TestCube.h"

class Application {
public:
	Application();
	~Application();

	int Run();

	void ShowDemoUI();

private:
	GUIManager m_guiManager;

	Window m_window;
	Timer m_timer;

	Camera camera;
	Light light;

	Model nano{ m_window.getRenderer(), "../Models/nanosuit/nanosuit.obj" };
	TestPlane plane;
	TestCube cube;

	bool showUI = true;
	bool showDemoUI = false;
	float simulationSpeed = 1.0f;

	void RenderFrame();
};