cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("TopSystems")

include(CMake/utils.cmake)

# include OpenGL
find_package (OpenGL REQUIRED)

# set external dependencies target list
set (dependenciesList
    OpenGL::GL
    glad
    GLFWPP
    imgui
)

# set internal sources targets list
set (sourcesList
    model
    view
    controller
)


set (DependenciesDir ${PROJECT_SOURCE_DIR}/Dependencies)
add_subdirectory (${DependenciesDir})

# Create list of GLSL shaders to binary
embed_resources(MyResources
    Resources/glsl/1D/frag.glsl
    Resources/glsl/1D/vert.glsl
    Resources/glsl/1D/white_vert.glsl)

# Create target
add_executable (${PROJECT_NAME}
    "TopSystemsTest/TopSystemsTest.cpp")
set_target_properties (${PROJECT_NAME}
    PROPERTIES
    CXX_STANDARD 20)

# Include embed-resource
target_include_directories (${PROJECT_NAME}
    PUBLIC TopSystemsTest
)

add_subdirectory ("TopSystemsTest")

# link libraries
foreach (library ${dependenciesList} ${sourcesList})
    target_link_libraries (${PROJECT_NAME} PRIVATE ${library})
endforeach ()