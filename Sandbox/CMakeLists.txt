# CMakeList.txt : CMake project for NoctalEngine, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Sandbox "main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Sandbox PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries("Sandbox" PUBLIC "NoctalEngine")

# TODO: Add tests and install targets if needed.
