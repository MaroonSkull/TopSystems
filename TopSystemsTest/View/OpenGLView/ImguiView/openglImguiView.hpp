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
	GUI UI_{};
	Shader* Vertex_ = nullptr;
	Shader* Fragment_ = nullptr;
	GLProgram* Pipeline_ = nullptr;
	GLsizei frameWidth_{};
	GLsizei frameHeight_{};
	GLuint VAO_{}; // vertex array object
	GLuint VBO_{}; // vertex buffer object
	GLuint FBO_{}; // frame buffer object
	GLuint RBO_{}; // rendering buffer object
	GLuint textureId_{}; // the texture id we'll need later to create a texture
	bool isRescaled_{ false };

	//creates the vertex arrays and buffers
	void create_triangle();
	void create_framebuffer();
	void rescale_framebuffer();

	void renderImgui(ImTextureID renderTexture);
public:
	// init glfwpp, glad, window, imgui
	OpenglImguiView();
	~OpenglImguiView();

	void draw();
	inline bool shouldClose() const;
};