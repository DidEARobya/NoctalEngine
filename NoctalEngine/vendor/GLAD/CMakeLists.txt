# CMakeList.txt : CMake project for GLAD, include source and define
# project specific logic here.
#

# Add source to this project's executable.
project(GLAD)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GLAD PROPERTY CXX_STANDARD 20)
endif()

add_library (GLAD PRIVATE "src/glad.c" "include/glad/glad.h")
target_link_libraries(NoctalEngine PRIVATE GLAD)
target_include_directories(GLAD PUBLIC include/)

# TODO: Add tests and install targets if needed.
