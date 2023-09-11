cmake_minimum_required (VERSION 3.8)
project (${PROJECT_NAME})

add_subdirectory ("Model")
add_subdirectory ("View")
add_subdirectory ("Controller")

set_target_properties (
	model
	view
		PROPERTIES
			CXX_STANDARD 20
)