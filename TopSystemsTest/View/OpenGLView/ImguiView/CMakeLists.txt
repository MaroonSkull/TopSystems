cmake_minimum_required (VERSION 3.8)
project (${PROJECT_NAME})


add_library (view
	openglImguiView.cpp
)

target_link_libraries(view
	PUBLIC
		IView
		Shader
		GLProgram
	PRIVATE
		glad
		GLFWPP
		imgui
		model
		controller
)

target_include_directories (view
    PUBLIC
		${DependenciesDir}/embed-resource
	PRIVATE
		./
)