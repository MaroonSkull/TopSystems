cmake_minimum_required (VERSION 3.8)
project (${PROJECT_NAME})


add_library (controller "openglImguiController.cpp")

target_include_directories (controller
	PRIVATE ./)
