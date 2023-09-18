#include <spdlog/spdlog.h>
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
		hints.apply();

		Window_ = new glfw::Window{ 640, 480, "Interview test" };
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
		//Pipeline_->useProgram();

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

void OpenglImguiView::renderImgui(std::function<ImTextureID(ImVec2)> renderTexture) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	UI_.DrawGUI(renderTexture);

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
	// отрисовываем GUI вместе с FBO
	// Здесь каждый раз создаётся новый буффер, но пока оставим как есть
	// Стоит вынести его инициализацию в отдельный коллбэк, если размер frameSize будет меняться
	renderImgui([&](ImVec2 frameSize) -> ImTextureID {
		GLsizei frameWidth = frameSize.x;
		GLsizei frameHeight = frameSize.y;

		// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
		GLuint FramebufferName = 0;
		glGenFramebuffers(1, &FramebufferName);
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

		// The texture we're going to render to
		GLuint renderedTexture;
		glGenTextures(1, &renderedTexture);

		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, renderedTexture);

		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameWidth, frameHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

		// Poor filtering. Needed !
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// The depth buffer
		GLuint depthrenderbuffer;
		glGenRenderbuffers(1, &depthrenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, frameWidth, frameHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

		// Set "renderedTexture" as our colour attachement #0
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

		// Set the list of draw buffers.
		GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

		// Always check that our framebuffer is ok
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::stringstream err;
			err << "framebuffer is corrupted, glCheckFramebufferStatus = " << glCheckFramebufferStatus(GL_FRAMEBUFFER);
			throw std::runtime_error{ err.str() };
		}
		// Render to our framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		glViewport(0, 0, frameWidth, frameHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right

		//-----------------------------------
		float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};




		return (ImTextureID)renderedTexture;
		return (ImTextureID)(GLuint)348585;
	});

	// меняем буфер для следующего раза
	Window_->swapBuffers();
}

inline bool OpenglImguiView::shouldClose() const {
	return Window_->shouldClose();
}