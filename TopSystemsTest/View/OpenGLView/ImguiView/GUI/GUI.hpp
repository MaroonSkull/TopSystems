#pragma once
#include <glfwpp/glfwpp.h>
#include <imgui.h>
#include <imgui_internal.h> // для docking
#include <functional>

class GUI {
private:
	ImGuiDockNodeFlags dockFlags_ = ImGuiDockNodeFlags_None;
	ImGuiID dockId_{}; // id родительского окна, к которому будем цеплять все прочие панели
	ImGuiID dockIdTools, dockIdLog, dockIdMouse;
	ImTextureID texture;

	bool circleSegmentsOverride = false;
	static const int circleSegmentsOverride_v_default = 15;
	int circleSegmentsOverride_v = circleSegmentsOverride_v_default;

	bool curveSegmentsOverride = false;
	static const int curveSegmentsOverride_v_default = 15;
	int curveSegmentsOverride_v = curveSegmentsOverride_v_default;


	void ShowMainMenuBar();
	void ShowDockSpace();
	void ShowLog();
	void ShowSidePanel();
	void ShowCanvas(std::function<ImTextureID(ImVec2)> getTexture);
	void ShowSimpleOverlay();
public:
	void DrawGUI(std::function<ImTextureID(ImVec2)> getTexture);
};