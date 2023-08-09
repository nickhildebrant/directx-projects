#pragma once
#include <set>

#include "GUIManager.h"
#include "Window.h"
#include "Timer.h"
#include "Model.h"

#include "Camera.h"
#include "Light.h"

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

	Model nano{ m_window.getRenderer(), "../Models/nano.gltf" };

	bool showUI = true;
	float simulationSpeed = 1.0f;

	void RenderFrame();
};