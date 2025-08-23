# CMakeList.txt : CMake project for NoctalEngine, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (NoctalEngine STATIC "src/NoctalEngine.cpp" "src/NoctalEngine.h" "src/nepch.h" "src/nepch.cpp" "src/NoctalEngine/Debugging/Logger.h" "src/NoctalEngine/Debugging/Logger.cpp" "src/NoctalEngine/Core.h" "src/NoctalEngine/EntryPoint.h" "src/NoctalEngine/Application/Application.h" "src/NoctalEngine/Application/Application.cpp" "src/NoctalEngine/Window/Window.h"   "src/NoctalEngine/Events/Event.h" "src/NoctalEngine/Events/KeyboardEvents.h" "src/NoctalEngine/Events/MouseEvents.h" "src/NoctalEngine/Rendering/Renderer.h" "src/NoctalEngine/Rendering/SDL/SDLRenderer.h" "src/NoctalEngine/Rendering/SDL/SDLRenderer.cpp" "src/NoctalEngine/Rendering/WrappedRenderer.h" "src/NoctalEngine/Rendering/Renderer.cpp" "src/NoctalEngine/Window/WindowManager.h" "src/NoctalEngine/Window/WindowManager.cpp" "src/NoctalEngine/Input/InputManager.h" "src/NoctalEngine/Input/InputManager.cpp" "src/NoctalEngine/Input/Layer.h" "src/NoctalEngine/Input/Layer.cpp" "src/NoctalEngine/Input/LayerStack.h" "src/NoctalEngine/Input/LayerStack.cpp" "src/NoctalEngine/Rendering/OpenGL/OpenGLRenderer.h" "src/NoctalEngine/Rendering/OpenGL/OpenGLRenderer.cpp" "src/NoctalEngine/Window/OpenGL/OpenGLWindow.h" "src/NoctalEngine/Window/OpenGL/OpenGLWindow.cpp" "src/NoctalEngine/Window/Window.cpp" "src/NoctalEngine/Debugging/ImGuiLayer.h" "src/NoctalEngine/Debugging/ImGuiLayer.cpp" "src/NoctalEngine/Utility/Timer.h" "src/NoctalEngine/Utility/Timer.cpp" "src/NoctalEngine/Input/KeyCodes.h")
set_property(TARGET NoctalEngine PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET NoctalEngine PROPERTY CXX_STANDARD 20)
endif()

set_target_properties(NoctalEngine PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${outputdir}/${PROJECT_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${outputdir}/${PROJECT_NAME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${outputdir}/${PROJECT_NAME})
set(CMAKE_OBJECT_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/int/${outputdir}/${PROJECT_NAME})

target_precompile_headers(NoctalEngine PUBLIC "src/nepch.h")

set(SDL_SHARED OFF)
set(SDL_STATIC ON)
set(SDL_TEST OFF)
set(SPDLOG_BUILD_SHARED OFF)
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/spdlog)
add_subdirectory(${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/SDL3)
add_subdirectory(${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/GLAD)
add_subdirectory(${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/ImGui)

target_link_libraries(NoctalEngine PUBLIC 
                            ImGui 
                            spdlog::spdlog 
                            SDL3::SDL3-static 
                            GLAD
)

target_include_directories(NoctalEngine PUBLIC 
                    ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/src
                    ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/
                    ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/GLAD/include/
                    ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/ImGui
                    ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/glm
)

target_include_directories(ImGui PUBLIC 
                    ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/SDL3/include
                    ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/ImGui
)

target_compile_definitions(NoctalEngine PUBLIC NE_PLATFORM_WINDOWS NE_BUILD_DLL NE_ENABLE_ASSERTS)
# TODO: Add tests and install targets if needed.
