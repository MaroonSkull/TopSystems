cmake_minimum_required (VERSION 3.8)
project (${PROJECT_NAME})


add_library (Shader
	Shader.cpp
)
target_include_directories (Shader
	PUBLIC
		${DependenciesDir}/embed-resource
		./
)
target_link_libraries (Shader
	PUBLIC
		glad
)
target_compile_features (Shader
	PUBLIC
		cxx_std_17
)