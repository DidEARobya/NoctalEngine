# CMakeList.txt : CMake project for NoctalEngine, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (NoctalEngine SHARED "NoctalEngine.cpp" "NoctalEngine.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET NoctalEngine PROPERTY CXX_STANDARD 20)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${outputdir}/${PROJECT_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${outputdir}/${PROJECT_NAME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${outputdir}/${PROJECT_NAME})
set(CMAKE_OBJECT_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/int/${outputdir}/${PROJECT_NAME})

add_custom_command(TARGET NoctalEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy 
        $<TARGET_FILE:NoctalEngine>
        ${PROJECT_BINARY_DIR}/Sandbox
)

# TODO: Add tests and install targets if needed.
