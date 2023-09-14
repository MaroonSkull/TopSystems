cmake_minimum_required (VERSION 3.8)
project (${PROJECT_NAME})


add_library (controller STATIC openglImguiController.cpp)

target_compile_features (controller PUBLIC cxx_std_17)

target_include_directories (controller PRIVATE ./)