cmake_minimum_required (VERSION 3.8)
project (${PROJECT_NAME})


add_library (model "flatFigureModel.cpp")

target_include_directories (model
	PRIVATE ./)
	
