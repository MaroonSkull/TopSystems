# CMakeList.txt : CMake project for TopSystemsTest, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (TopSystemsTest "TopSystemsTest.cpp" "TopSystemsTest.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TopSystemsTest PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
