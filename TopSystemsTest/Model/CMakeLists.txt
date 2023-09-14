cmake_minimum_required (VERSION 3.8)
project (${PROJECT_NAME})


add_library (model STATIC flatFigureModel.cpp)

target_compile_features (model PUBLIC cxx_std_17)

target_include_directories (model PRIVATE ./)