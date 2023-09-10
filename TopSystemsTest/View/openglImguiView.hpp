#pragma once
#include <glad/gl.h>
#include <glfwpp/glfwpp.h>
#include <imgui.h>
#include <Resource.h>

#include <IView.hpp>
// init glfwpp
// init glad
// init window
// init imgui

void view();

class OpenglImguiView : public IView {

};