# CMakeList.txt : CMake project for NoctalEngine, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (NoctalEngine SHARED "src/NoctalEngine.cpp" "src/NoctalEngine.h" "src/nepch.h" "src/nepch.cpp")

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

target_precompile_headers(NoctalEngine PUBLIC "src/nepch.h")

add_subdirectory(${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/spdlog)
target_link_libraries(NoctalEngine PUBLIC spdlog::spdlog)

add_subdirectory(${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/SDL3)
target_link_libraries(NoctalEngine PUBLIC SDL3::SDL3)

target_include_directories(NoctalEngine PUBLIC 
                    ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/src
                    ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/
)

# TODO: Add tests and install targets if needed.
