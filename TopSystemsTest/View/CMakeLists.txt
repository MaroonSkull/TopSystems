cmake_minimum_required (VERSION 3.8)
project (${PROJECT_NAME})


add_library(IView INTERFACE)
target_compile_features(IView INTERFACE cxx_std_17)
target_include_directories(IView INTERFACE ./)

add_subdirectory ("OpenGLView")