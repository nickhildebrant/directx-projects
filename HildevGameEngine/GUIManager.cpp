#include "GUIManager.h"
#include "ImGUI/imgui.h"

GUIManager::GUIManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

GUIManager::~GUIManager()
{
	ImGui::DestroyContext();
}