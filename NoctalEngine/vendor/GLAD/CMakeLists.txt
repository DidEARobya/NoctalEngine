# CMakeList.txt : CMake project for GLAD, include source and define
# project specific logic here.
#

# Add source to this project's executable.
project(GLAD)
add_library(GLAD include/glad/glad.h src/glad.c)

target_include_directories(GLAD PUBLIC include/)

# TODO: Add tests and install targets if needed.
