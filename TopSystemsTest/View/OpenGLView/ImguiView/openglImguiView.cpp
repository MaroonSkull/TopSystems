#include <iostream>
#include <sstream>
#include <openglImguiView.hpp>

OpenglImguiView::OpenglImguiView() {
	try {
		// RAII GLFW
		auto GLFW = glfw::init();

		// Window
		glfw::WindowHints hints;
		hints.clientApi = glfw::ClientApi::OpenGl;
		hints.contextVersionMajor = 3;
		hints.contextVersionMinor = 3;
		hints.apply();

		glfw::Window window{ 640, 480, "Interview test" };
		glfw::makeContextCurrent(window);

		int version = gladLoadGL(glfw::getProcAddress);
		if (version == 0)
			throw std::runtime_error{"Failed to initialize OpenGL context"};

		std::cout << "Loaded OpenGL " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

		// Shaiders
		Fragment_ = new Shader(LOAD_RESOURCE(Resources_glsl_1D_frag_glsl), Shader::Fragment);
		Vertex_ = new Shader(LOAD_RESOURCE(Resources_glsl_1D_vert_glsl), Shader::Vertex);
		const Shader* Vertex2_ = new Shader(LOAD_RESOURCE(Resources_glsl_1D_white_vert_glsl), Shader::Vertex);

		Pipeline_ = new GLProgram();
		Pipeline_->attachShader(Fragment_);
		//Pipeline_->attachShader(Vertex_);
		Pipeline_->attachShader(Vertex2_);

		Pipeline_->linkProgram();
		Pipeline_->useProgram();
	}
	catch (const glfw::Error& e) {
		// todo replace "View" with file name macro
		std::cerr << "glfwpp exception in View!" << std::endl;
		throw e;
	}
}

OpenglImguiView::~OpenglImguiView() {
	delete Pipeline_;

	delete Vertex_;
	delete Fragment_;
}

