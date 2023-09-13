#include <GUI.hpp>

void GUI::ShowMainMenuBar() {
    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("Menu")) {
            if(ImGui::BeginMenu("Examples")) {
                if(ImGui::MenuItem("1")) {}
                if(ImGui::MenuItem("2")) {}
                if(ImGui::MenuItem("3")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Edit")) {
            if(ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if(ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("View")) {
            if(ImGui::MenuItem("Grid")) {}
            if(ImGui::MenuItem("Sized", NULL, false, false)) {}  // Disabled item
            ImGui::Separator();
            
            if(ImGui::Checkbox("##circlesegmentoverride", &circleSegmentsOverride))
                circleSegmentsOverride_v = circleSegmentsOverride_v_default;
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            circleSegmentsOverride |= ImGui::SliderInt("Circle segments override", &circleSegmentsOverride_v, 3, 40);
            if(ImGui::Checkbox("##curvessegmentoverride", &curveSegmentsOverride))
                curveSegmentsOverride_v = curveSegmentsOverride_v_default;
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            curveSegmentsOverride |= ImGui::SliderInt("Curves segments override", &curveSegmentsOverride_v, 3, 40);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void GUI::ShowDockSpace() {
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}

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

	ImGui::ShowDemoWindow();
}
