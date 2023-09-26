﻿#include <spdlog/spdlog.h>
#include <sstream>
#include <openglImguiView.hpp>

OpenglImguiView::OpenglImguiView() : GLFW_(glfw::init()) {
	try {
		// Window
		glfw::WindowHints hints;
		hints.clientApi = glfw::ClientApi::OpenGl;
		hints.contextVersionMajor = 3;
		hints.contextVersionMinor = 3;
		hints.openglProfile = glfw::OpenGlProfile::Core;
		hints.openglForwardCompat = true;
		hints.apply();

		Window_ = new glfw::Window{ 640, 480, "Interview test" };
		// первый проход фреймбуфера рендерим в размерах самого окна.
		std::tie(frameWidth_, frameHeight_) = Window_->getFramebufferSize();
		glfw::makeContextCurrent(*Window_);
		glfw::swapInterval(1); // Enable vsync

		int version = gladLoadGL(glfw::getProcAddress);
		if (version == 0)
			throw std::runtime_error{ "Failed to initialize OpenGL context" };

		spdlog::info("Loaded OpenGL {}.{}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

		// Shaiders
		Fragment_ = new Shader(LOAD_RESOURCE(Resources_glsl_1D_frag_glsl), Shader::Fragment);
		Vertex_ = new Shader(LOAD_RESOURCE(Resources_glsl_1D_vert_glsl), Shader::Vertex);
		//Vertex_ = new Shader(LOAD_RESOURCE(Resources_glsl_1D_white_vert_glsl), Shader::Vertex);

		Pipeline_ = new GLProgram();
		Pipeline_->attachShader(Fragment_);
		Pipeline_->attachShader(Vertex_);

		Pipeline_->linkProgram();
		Fragment_->deleteShader();
		Vertex_->deleteShader();

		// Imgui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle &style = ImGui::GetStyle();
		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		// Setup Platform/Renderer backends
		if (!ImGui_ImplGlfw_InitForOpenGL(*Window_, true))
			throw std::runtime_error{ "ImGui_ImplGlfw_InitForOpenGL return false." };
		if (!ImGui_ImplOpenGL3_Init("#version 330"))
			throw std::runtime_error{ "ImGui_ImplOpenGL3_Init return false." };

		glViewport(0, 0, frameWidth_, frameHeight_);
		create_triangle();
		create_framebuffer();
	}
	catch (const glfw::Error& e) {
		// todo replace "View" with file name macro
		spdlog::error("glfwpp exception in View!");
		throw e;
	}
}

OpenglImguiView::~OpenglImguiView() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	delete Pipeline_;

	delete Vertex_;
	delete Fragment_;

	delete Window_;
	// GlfwLibrary destructor calls glfwTerminate automatically
}

void OpenglImguiView::create_triangle() {
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f, // 1. vertex x, y, z
		1.0f, -1.0f, 0.0f, // 2. vertex ...
		0.0f, 1.0f, 0.0f // etc... 
	};

	glGenVertexArrays(1, &VAO_);
	glBindVertexArray(VAO_);

	glGenBuffers(1, &VBO_);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void OpenglImguiView::create_framebuffer() {
	glGenFramebuffers(1, &FBO_);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_);

	// Текстура, в которую мы будем рендерить
	glGenTextures(1, &textureId_);
	// Рендербуфер для целей OpenGL
	glGenRenderbuffers(1, &RBO_);
	rescale_framebuffer();

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::stringstream err;
		err << "framebuffer is not complete, glCheckFramebufferStatus = " << glCheckFramebufferStatus(GL_FRAMEBUFFER);
		throw std::runtime_error{ err.str() };
	}

	// unbind it all
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void OpenglImguiView::rescale_framebuffer() {
	glBindTexture(GL_TEXTURE_2D, textureId_);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameWidth_, frameHeight_, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId_, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, RBO_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameWidth_, frameHeight_);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO_);
}

void OpenglImguiView::renderImgui(ImTextureID renderTexture) {
	ImGui::NewFrame();

	auto sizes = UI_.DrawGUI(renderTexture);
	if (frameWidth_ != sizes.x || frameHeight_ != sizes.y) {
		isRescaled_ = true;
		frameWidth_ = sizes.x;
		frameHeight_ = sizes.y;
	}

	ImGui::Render();
}

void OpenglImguiView::draw() {
	// отрисовываем GUI вместе с тем, что лежит в FBO с прошлого кадра
	// решение некрасивое, но альтернатив для imgui я пока не нашёл
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	renderImgui((ImTextureID)NULL);
		
	// Render on the whole framebuffer
	glViewport(0, 0, frameWidth_, frameHeight_);
	// Render to our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_);


	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		glfw::Window& backupCurrentContext = glfw::getCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfw::makeContextCurrent(backupCurrentContext);
	}

	// меняем буфер для следующего раза
	Window_->swapBuffers();
}

inline bool OpenglImguiView::shouldClose() const {
	return Window_->shouldClose();
}