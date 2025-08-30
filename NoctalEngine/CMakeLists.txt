# CMakeList.txt : CMake project for NoctalEngine, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (NoctalEngine STATIC "src/NoctalEngine.cpp" "src/NoctalEngine.h" "src/nepch.h" "src/nepch.cpp" "src/NoctalEngine/Debugging/Logger.h" "src/NoctalEngine/Debugging/Logger.cpp" "src/NoctalEngine/Core.h" "src/NoctalEngine/EntryPoint.h" "src/NoctalEngine/Application/Application.h" "src/NoctalEngine/Application/Application.cpp" "src/NoctalEngine/Window/Window.h"   "src/NoctalEngine/Events/Event.h" "src/NoctalEngine/Events/KeyboardEvents.h" "src/NoctalEngine/Events/MouseEvents.h" "src/NoctalEngine/Rendering/Renderer.h" "src/NoctalEngine/Rendering/WrappedRenderer.h" "src/NoctalEngine/Rendering/Renderer.cpp" "src/NoctalEngine/Window/WindowManager.h" "src/NoctalEngine/Window/WindowManager.cpp" "src/NoctalEngine/Input/InputManager.h" "src/NoctalEngine/Input/InputManager.cpp" "src/NoctalEngine/Input/Layer.h" "src/NoctalEngine/Input/Layer.cpp" "src/NoctalEngine/Input/LayerStack.h" "src/NoctalEngine/Input/LayerStack.cpp" "src/NoctalEngine/Rendering/OpenGL/OpenGLRenderer.h" "src/NoctalEngine/Rendering/OpenGL/OpenGLRenderer.cpp" "src/NoctalEngine/Window/OpenGL/OpenGLWindow.h" "src/NoctalEngine/Window/OpenGL/OpenGLWindow.cpp" "src/NoctalEngine/Window/Window.cpp" "src/NoctalEngine/Debugging/ImGuiLayer.h" "src/NoctalEngine/Debugging/ImGuiLayer.cpp" "src/NoctalEngine/Utility/Timer.h" "src/NoctalEngine/Utility/Timer.cpp" "src/NoctalEngine/Input/KeyCodes.h" "src/NoctalEngine/Rendering/Buffers/IndexBuffer.h" "src/NoctalEngine/Rendering/OpenGL/Buffers/OpenGLIndexBuffer.h" "src/NoctalEngine/Rendering/OpenGL/Buffers/OpenGLIndexBuffer.cpp" "src/NoctalEngine/Rendering/Buffers/VertexBuffer.h" "src/NoctalEngine/Rendering/OpenGL/Buffers/OpenGLVertexBuffer.h" "src/NoctalEngine/Rendering/OpenGL/Buffers/OpenGLVertexBuffer.cpp" "src/NoctalEngine/Rendering/Bindable.h" "src/NoctalEngine/Rendering/OpenGL/Shaders/OpenGLShaderProgram.h" "src/NoctalEngine/Rendering/OpenGL/Shaders/OpenGLShaderProgram.cpp" "src/NoctalEngine/Rendering/Shaders/Shader.h" "src/NoctalEngine/Rendering/Buffers/BufferLayout.h" "src/NoctalEngine/Rendering/Drawables/Drawable.h" "src/NoctalEngine/Rendering/Drawables/Drawable.cpp" "src/NoctalEngine/Rendering/OpenGL/Drawables/OpenGLDrawable.h" "src/NoctalEngine/Rendering/OpenGL/Drawables/OpenGLDrawable.cpp" "src/NoctalEngine/Rendering/Drawables/DrawableBase.h"  "src/NoctalEngine/Rendering/Geometry/Geometry.h" "src/NoctalEngine/Rendering/Camera/OrthographicCamera.h" "src/NoctalEngine/Rendering/Camera/OrthographicCamera.cpp" "src/NoctalEngine/Application/AppLayer.h" "src/NoctalEngine/Rendering/Buffers/TransformCBuffer.h"   "src/NoctalEngine/Rendering/Materials/Material.h" "src/NoctalEngine/Rendering/Materials/Material.cpp" "src/NoctalEngine/Rendering/Textures/Texture.cpp" "src/NoctalEngine/Rendering/Textures/Texture2D.h" "src/NoctalEngine/Rendering/Textures/Texture2D.cpp" "src/NoctalEngine/Rendering/OpenGL/Textures/OpenGLTexture2D.h" "src/NoctalEngine/Rendering/OpenGL/Textures/OpenGLTexture2D.cpp"   "vendor/stb_image/stb_image.h" "vendor/stb_image/stb_image.cpp"   "src/NoctalEngine/Rendering/OpenGL/Shaders/OpenGLFragmentShader.h" "src/NoctalEngine/Rendering/OpenGL/Shaders/OpenGLFragmentShader.cpp" "src/NoctalEngine/Rendering/OpenGL/Shaders/OpenGLVertexShader.cpp" "src/NoctalEngine/Rendering/OpenGL/Shaders/OpenGLVertexShader.h" "src/NoctalEngine/Rendering/Shaders/ShaderLibrary.h" "src/NoctalEngine/Rendering/Shaders/ShaderLibrary.cpp" "src/NoctalEngine/Rendering/Camera/OrthographicCameraController.h" "src/NoctalEngine/Rendering/Camera/OrthographicCameraController.cpp" "src/NoctalEngine/Rendering/Buffers/ConstantBuffer.h" "src/NoctalEngine/Rendering/OpenGL/Buffers/OpenGLUniformBufferObject.h" "src/NoctalEngine/Rendering/OpenGL/Buffers/OpenGLUniformBufferObject.cpp" )

set(CMAKE_MSVC_RUNTIME_LIBRARY 
    "$<$<CONFIG:Debug>:MultiThreadedDebug>;$<$<CONFIG:Release>:MultiThreaded>"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET NoctalEngine PROPERTY CXX_STANDARD 20)
endif()

set_target_properties(NoctalEngine PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${PROJECT_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${PROJECT_NAME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${PROJECT_NAME})
set(CMAKE_OBJECT_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/int/$<CONFIG>/${PROJECT_NAME})

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
                    ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/stb_image
)

target_include_directories(ImGui PUBLIC 
                    ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/SDL3/include
                    ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/ImGui
)

set(PROJECT_DIT_PATH "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/")
set(ASSET_DIR_PATH "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/Assets/")
target_compile_definitions(NoctalEngine PUBLIC NE_PLATFORM_WINDOWS NE_BUILD_DLL NE_ENABLE_ASSERTS NE_ENABLE_PROFILER ASSET_DIR="${ASSET_DIR_PATH}" PROJECT_DIR="${PROJECT_DIR_PATH}")

# TODO: Add tests and install targets if needed.
