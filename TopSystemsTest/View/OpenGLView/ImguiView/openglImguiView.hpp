#pragma once
#include <glad/gl.h>
#include <glfwpp/glfwpp.h>
#include <imgui.h>
#include <Resource.h>

#include <IView.hpp>
#include <Shader.hpp>
#include <GLProgram.hpp>



class OpenglImguiView final : public IView {
private:
	Shader *Vertex_ = nullptr;
	Shader *Fragment_ = nullptr;

	GLProgram* Pipeline_ = nullptr;
public:
	// init glfwpp, glad, window, imgui
	OpenglImguiView();
	~OpenglImguiView();
};