#include <openglImguiView.hpp>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <sstream>
#include <glm/ext/matrix_clip_space.hpp>

OpenglImguiView::OpenglImguiView(FlatFigureModel* pModel, IController* pController) : GLFW_(glfw::init()) {
	try {
		// Window
		glfw::WindowHints hints;
		hints.clientApi = glfw::ClientApi::OpenGl;
		hints.openglProfile = glfw::OpenGlProfile::Core;
		hints.contextVersionMajor = 3;
		hints.contextVersionMinor = 3;
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


		glViewport(0, 0, frameWidth_, frameHeight_);
		create_triangle();

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

		create_framebuffer();

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

	glDeleteFramebuffers(1, &FBO_);
	glDeleteTextures(1, &textureId_);
	glDeleteRenderbuffers(1, &RBO_);
	// todo glDeleteVertexArrays(...

	delete Pipeline_;

	delete Vertex_;
	delete Fragment_;

	delete Window_;
	// GlfwLibrary destructor calls glfwTerminate automatically
}

void OpenglImguiView::create_triangle() {
	vertices_ = { {
		-1.0f, -1.0f, 0.0f, // 1. vertex x, y, z
		1.0f, -1.0f, 0.0f, // 2. vertex ...
		0.0f, 1.0f, 0.0f // etc... 
	} };

	glGenVertexArrays(1, &VAO_);
	glBindVertexArray(VAO_);

	glGenBuffers(1, &VBO_);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	// GL_DYNAMIC_DRAW - будем интенсивно менять данные.
	// Можно ещё попробовать STREAM
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(vertices_.at(0)), vertices_.data(), GL_DYNAMIC_DRAW);

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

void OpenglImguiView::draw() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	// reinterpret_cast тут неизбежен, это вынужденная мера, чтобы передать opengl текстуру из GLFWPP в imgui
	auto [frameSizes, mousePosition] = UI_.DrawGUI(reinterpret_cast<ImTextureID>(textureId_));
	
	// Render on the whole framebuffer
	glViewport(0, 0, frameWidth_, frameHeight_);
	// Render to our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_);
	// Rescale binded framebuffer
	if (frameWidth_ != frameSizes.x || frameHeight_ != frameSizes.y) {
		frameWidth_ = frameSizes.x;
		frameHeight_ = frameSizes.y;
		rescale_framebuffer();
	}

	if (mousePosition) {
		// передаём в контроллер в обязательном порядке, пусть он разбирается
		// convert to normalised coords via glm
		auto &&[x, y] = mousePosition.value();
		glm::mat4 ortho = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
		vertices_.at(0) = x / frameWidth_;
		vertices_.at(1) = y / frameHeight_;
	}
	

	glClearColor(0.9f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	Pipeline_->useProgram();

	glBindVertexArray(VAO_);
	
	if (mousePosition) {
		// получаем данные из модели и отправляем в опенгл
		glBindBuffer(GL_ARRAY_BUFFER, VBO_);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_.size() * sizeof(vertices_.at(0)), vertices_.data());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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