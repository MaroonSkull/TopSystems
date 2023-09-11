cmake_minimum_required (VERSION 3.8)
project (${PROJECT_NAME})


add_library (GLProgram
	GLProgram.cpp
)
target_include_directories (GLProgram
	PUBLIC
		./
)
target_link_libraries (GLProgram
	PUBLIC
		Shader
)