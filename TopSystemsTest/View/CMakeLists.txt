cmake_minimum_required (VERSION 3.8)
project (${PROJECT_NAME})

#Create library target
add_library(IView INTERFACE)
target_include_directories(IView
	INTERFACE
		${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(IView
	INTERFACE
		cxx_std_17)

add_library (view
	"openglImguiView.cpp"
	${MyResources}
)

target_link_libraries(view
	PUBLIC
		IView
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