# here we add all dependencies
cmake_minimum_required (VERSION 3.19)
project (${PROJECT_NAME} LANGUAGES CXX)

#glad
add_subdirectory_checked (${CMAKE_CURRENT_SOURCE_DIR}/glad)

# GLFWPP
# glfwpp has public linkage of glfw
set(GLFWPP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # disable building examples
add_subdirectory_checked (${CMAKE_CURRENT_SOURCE_DIR}/glfwpp)

# imgui
add_library (imgui
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    imgui
    imgui/backends
)
target_link_libraries(imgui glfw glad)

if (WIN32 AND NOT (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC"))
    target_link_libraries(imgui "Imm32.lib")
endif()

#embed-resource
add_subdirectory_checked (${CMAKE_CURRENT_SOURCE_DIR}/embed-resource)