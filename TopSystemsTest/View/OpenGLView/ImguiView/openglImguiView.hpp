#pragma once
#include <glad/gl.h>
#include <glfwpp/glfwpp.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GUI.hpp>
#include <Resource.h>

#include <IView.hpp>
#include <Shader.hpp>
#include <GLProgram.hpp>



class OpenglImguiView final : public IView {
private:
	glfw::GlfwLibrary GLFW_; // RAII
	glfw::Window* Window_ = nullptr;
	GUI UI_;
	Shader *Vertex_ = nullptr;
	Shader *Fragment_ = nullptr;
	GLProgram* Pipeline_ = nullptr;


	void renderImgui();
public:
	// init glfwpp, glad, window, imgui
	OpenglImguiView();
	~OpenglImguiView();

	void draw();
	inline bool shouldClose() const;
};