cmake_minimum_required (VERSION 3.8)
project (${PROJECT_NAME})


add_subdirectory ("GUI")

add_library (view
	openglImguiView.cpp
)

target_link_libraries(view
	PUBLIC
		IView
		Shader
		GLProgram
		GUI
	PRIVATE
		EmbedResource
		glad
		GLFWPP
		imgui
		model
		controller
)

target_include_directories (view
	PRIVATE
		./
)