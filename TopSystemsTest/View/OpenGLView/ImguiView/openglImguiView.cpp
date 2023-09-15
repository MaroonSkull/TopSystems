#include <spdlog/spdlog.h>
#include <sstream>
#include <openglImguiView.hpp>

// todo проверить. Что, если glfwpp выбросит исключение из member initializer list?
OpenglImguiView::OpenglImguiView() : GLFW_(glfw::init()) {
	try {
		// Window
		glfw::WindowHints hints;
		hints.clientApi = glfw::ClientApi::OpenGl;
		hints.contextVersionMajor = 3;
		hints.contextVersionMinor = 3;
		hints.openglProfile = glfw::OpenGlProfile::Core;
		hints.apply();

		Window_ = new glfw::Window{ 640, 480, "Interview test" };
		glfw::makeContextCurrent(*Window_);

		int version = gladLoadGL(glfw::getProcAddress);
		if (version == 0)
			throw std::runtime_error{ "Failed to initialize OpenGL context" };

		spdlog::info("Loaded OpenGL {}.{}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

		// Shaiders
		Fragment_ = new Shader(LOAD_RESOURCE(Resources_glsl_1D_frag_glsl), Shader::Fragment);
		//Vertex_ = new Shader(LOAD_RESOURCE(Resources_glsl_1D_vert_glsl), Shader::Vertex);
		Vertex_ = new Shader(LOAD_RESOURCE(Resources_glsl_1D_white_vert_glsl), Shader::Vertex);

		Pipeline_ = new GLProgram();
		Pipeline_->attachShader(Fragment_);
		Pipeline_->attachShader(Vertex_);

		Pipeline_->linkProgram();
		Pipeline_->useProgram();

		// Imgui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;

		if (!ImGui_ImplGlfw_InitForOpenGL(*Window_, true))
			throw std::runtime_error{ "ImGui_ImplGlfw_InitForOpenGL return false." };
		if (!ImGui_ImplOpenGL3_Init())
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

	delete Pipeline_;

	delete Vertex_;
	delete Fragment_;

	delete Window_;
	// GlfwLibrary destructor calls glfwTerminate automatically
}

void OpenglImguiView::renderImgui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	UI_.DrawGUI(Window_);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		glfw::Window& backupCurrentContext = glfw::getCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfw::makeContextCurrent(backupCurrentContext);
	}
}

void OpenglImguiView::draw() {
	// отрисовываем текущее состояние
	glClear(GL_COLOR_BUFFER_BIT);

	// отрисовываем GUI
	renderImgui();

	// меняем буфер для следующего раза
	Window_->swapBuffers();
}

inline bool OpenglImguiView::shouldClose() const {
	return Window_->shouldClose();
}