cmake_minimum_required (VERSION 3.8)
project (${PROJECT_NAME})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()
