#pragma once
#include <glfwpp/glfwpp.h>
#include <imgui.h>

class GUI {
private:
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
	void ShowCanvas();
	void ShowSimpleOverlay();
public:
	void DrawGUI(glfw::Window *Window);
};