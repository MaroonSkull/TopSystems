cmake_minimum_required (VERSION 3.13) #for target_sources relative patch

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("TopSystems"
    VERSION 0.1
    DESCRIPTION "Решение тестового задания от ЗАО \"Топ Системы\" на позицию \"Разработчик С++\""
    HOMEPAGE_URL "https://github.com/MaroonSkull/TopSystems"
    LANGUAGES CXX
)

include (CMake/utils.cmake)

#embed-resource
add_subdirectory_checked (${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/embed-resource)

# Create target
add_executable (${PROJECT_NAME})
set_target_properties (${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 17
)

# include OpenGL
find_package (OpenGL REQUIRED)

set (DependenciesDir ${PROJECT_SOURCE_DIR}/Dependencies)

add_subdirectory (${DependenciesDir})

# Include embed-resource
target_include_directories (${PROJECT_NAME}
    PUBLIC TopSystemsTest
)

add_subdirectory ("TopSystemsTest")

# Create list of GLSL shaders to binary

embed_resources(MyResources
    Resources/glsl/1D/frag.glsl
    Resources/glsl/1D/vert.glsl
    Resources/glsl/1D/white_vert.glsl
)

target_sources (${PROJECT_NAME} PUBLIC
    TopSystemsTest/TopSystemsTest.cpp
    ${MyResources}
)

# link libraries
foreach (library
# dependencies
    #OpenGL::GL
    glad
    GLFWPP
    imgui
# sources
    model
    view
    controller
)
    target_link_libraries (${PROJECT_NAME} PRIVATE ${library})
endforeach ()