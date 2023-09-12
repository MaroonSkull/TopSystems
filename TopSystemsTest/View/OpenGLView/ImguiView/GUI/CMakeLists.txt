cmake_minimum_required (VERSION 3.8)
project (${PROJECT_NAME})


add_library (GUI
	GUI.cpp
)

target_link_libraries(GUI
	PRIVATE
		glad
		GLFWPP
		imgui
		model
		controller
)

target_include_directories (GUI
	PUBLIC
		./
)