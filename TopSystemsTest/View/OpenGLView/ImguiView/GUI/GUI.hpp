#pragma once
#include <glfwpp/glfwpp.h>
#include <imgui.h>

class GUI {
private:
	glfw::Window *Window_ = nullptr;

	void ShowMainMenuBar();
	void ShowDockSpace();
	void ShowLog();
	void ShowSidePanel();
	void ShowCanvas();
	void ShowSimpleOverlay();
public:
	GUI() = default;
	~GUI() = default;

	void DrawGUI(glfw::Window *Window);
};