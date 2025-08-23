# CMakeList.txt : CMake project for Sandbox, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Sandbox "main.cpp")
set_property(TARGET Sandbox PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Sandbox PROPERTY CXX_STANDARD 20)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${outputdir}/${PROJECT_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${outputdir}/${PROJECT_NAME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${outputdir}/${PROJECT_NAME})
set(CMAKE_OBJECT_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/int/${outputdir}/${PROJECT_NAME})

target_link_libraries(Sandbox PUBLIC NoctalEngine)

target_include_directories(Sandbox PUBLIC 
                    ${CMAKE_SOURCE_DIR}/NoctalEngine
                    ${CMAKE_SOURCE_DIR}/NoctalEngine/vendor/glm/glm
)

# TODO: Add tests and install targets if needed.
