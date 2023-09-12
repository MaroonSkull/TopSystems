#include <GUI.hpp>



void GUI::ShowMainMenuBar() {}

void GUI::ShowDockSpace() {}

void GUI::ShowLog() {}

void GUI::ShowSidePanel() {}

void GUI::ShowCanvas() {}

void GUI::ShowSimpleOverlay() {}

void GUI::DrawGUI(glfw::Window *Window) {
	ShowMainMenuBar();
	ShowDockSpace();
	ShowLog();
	ShowSidePanel();
	ShowCanvas();
	ShowSimpleOverlay();
}
