# CMakeList.txt : CMake project for NoctalEngine, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (NoctalEngine STATIC
    "src/NoctalEngine.cpp"
    "src/NoctalEngine.h"
    "src/nepch.h"
    "src/nepch.cpp"
    "src/NoctalEngine/Debugging/Logger.h"
    "src/NoctalEngine/Debugging/Logger.cpp"
    "src/NoctalEngine/Core.h"
    "src/NoctalEngine/EntryPoint.h"
    "src/NoctalEngine/Application/Application.h"
    "src/NoctalEngine/Application/Application.cpp"
    "src/NoctalEngine/Window/Window.h"
    "src/NoctalEngine/Events/Event.h"
    "src/NoctalEngine/Events/KeyboardEvents.h"
    "src/NoctalEngine/Events/MouseEvents.h"
    "src/NoctalEngine/Rendering/Renderer.h"
    "src/NoctalEngine/Rendering/WrappedRenderer.h"
    "src/NoctalEngine/Rendering/Renderer.cpp"
    "src/NoctalEngine/Window/WindowManager.h"
    "src/NoctalEngine/Window/WindowManager.cpp"
    "src/NoctalEngine/Input/InputManager.h"
    "src/NoctalEngine/Input/InputManager.cpp"
    "src/NoctalEngine/Input/Layer.h"
    "src/NoctalEngine/Input/Layer.cpp"
    "src/NoctalEngine/Input/LayerStack.h"
    "src/NoctalEngine/Input/LayerStack.cpp"
    "src/NoctalEngine/Window/OpenGL/OpenGLWindow.h"
    "src/NoctalEngine/Window/OpenGL/OpenGLWindow.cpp"
    "src/NoctalEngine/Window/Window.cpp"
    "src/NoctalEngine/Debugging/ImGuiLayer.h"
    "src/NoctalEngine/Debugging/ImGuiLayer.cpp"
    "src/NoctalEngine/Utility/Timer.h"
    "src/NoctalEngine/Utility/Timer.cpp"
    "src/NoctalEngine/Input/KeyCodes.h"
    "src/NoctalEngine/Rendering/Buffers/IndexBuffer.h"
    "src/NoctalEngine/Rendering/Buffers/VertexBuffer.h"
    "src/NoctalEngine/Rendering/Bindable.h"
    "src/NoctalEngine/Rendering/Shaders/Shader.h"
    "src/NoctalEngine/Rendering/Buffers/BufferLayout.h"
    "src/NoctalEngine/Rendering/Drawables/Drawable.h"
    "src/NoctalEngine/Rendering/Drawables/Drawable.cpp"
    "src/NoctalEngine/Rendering/Drawables/DrawableBase.h"
    "src/NoctalEngine/Rendering/Camera/OrthographicCamera.h"
    "src/NoctalEngine/Rendering/Camera/OrthographicCamera.cpp"
    "src/NoctalEngine/Application/AppLayer.h"
    "src/NoctalEngine/Rendering/Materials/Material.h"
    "src/NoctalEngine/Rendering/Materials/Material.cpp"
    "src/NoctalEngine/Rendering/Textures/Texture.cpp"
    "src/NoctalEngine/Rendering/Textures/Texture2D.h"
    "src/NoctalEngine/Rendering/Textures/Texture2D.cpp"
    "vendor/stb_image/stb_image.h"
    "vendor/stb_image/stb_image.cpp"
    "src/NoctalEngine/Rendering/Shaders/ShaderLibrary.h"
    "src/NoctalEngine/Rendering/Shaders/ShaderLibrary.cpp"
    "src/NoctalEngine/Rendering/Camera/OrthographicCameraController.h"
    "src/NoctalEngine/Rendering/Camera/OrthographicCameraController.cpp"
    "src/NoctalEngine/Rendering/Buffers/ConstantBuffer.h"
    "src/NoctalEngine/Rendering/Geometry/IndexedTriangleList.h"
    "src/NoctalEngine/Rendering/Geometry/Quad.h"
    "src/NoctalEngine/Rendering/Geometry/Triangle.h"
    "src/NoctalEngine/Rendering/Geometry/Geometry.h"
    "src/NoctalEngine/Rendering/Geometry/VertexLayout.h"
    "src/NoctalEngine/Rendering/Geometry/VertexLayout.cpp"
    "src/NoctalEngine/Rendering/RendererConfig.h"
    "src/NoctalEngine/Rendering/Geometry/Bounds.h"
    "src/NoctalEngine/Rendering/Pipeline/Pipeline.h"
    "src/NoctalEngine/Rendering/Drawables/Shapes.h"
    "src/NoctalEngine/Rendering/Drawables/Shapes.cpp"
    "src/NoctalEngine/Rendering/Batching/BatchRenderer2D.h"
    "src/NoctalEngine/Rendering/Batching/BatchRenderer2D.cpp"
    "src/NoctalEngine/Rendering/D3D12/D3D12Common.h"
    "src/NoctalEngine/Rendering/D3D12/D3D12Renderer.h"
    "src/NoctalEngine/Rendering/D3D12/D3D12Renderer.cpp"
    "src/NoctalEngine/Rendering/D3D12/Descriptors/D3D12DescriptorHeap.h"
    "src/NoctalEngine/Rendering/D3D12/Descriptors/D3D12DescriptorHeap.cpp"
    "src/NoctalEngine/Rendering/D3D12/Buffers/D3D12UploadRing.h"
    "src/NoctalEngine/Rendering/D3D12/Buffers/D3D12UploadRing.cpp"
    "src/NoctalEngine/Rendering/D3D12/Buffers/D3D12VertexBuffer.h"
    "src/NoctalEngine/Rendering/D3D12/Buffers/D3D12VertexBuffer.cpp"
    "src/NoctalEngine/Rendering/D3D12/Buffers/D3D12IndexBuffer.h"
    "src/NoctalEngine/Rendering/D3D12/Buffers/D3D12IndexBuffer.cpp"
    "src/NoctalEngine/Rendering/D3D12/Buffers/D3D12ConstantBuffer.h"
    "src/NoctalEngine/Rendering/D3D12/Buffers/D3D12ConstantBuffer.cpp"
    "src/NoctalEngine/Rendering/D3D12/Shaders/D3D12Shader.h"
    "src/NoctalEngine/Rendering/D3D12/Shaders/D3D12Shader.cpp"
    "src/NoctalEngine/Rendering/D3D12/Pipeline/D3D12Pipeline.h"
    "src/NoctalEngine/Rendering/D3D12/Pipeline/D3D12Pipeline.cpp"
    "src/NoctalEngine/Rendering/D3D12/Textures/D3D12Texture2D.h"
    "src/NoctalEngine/Rendering/D3D12/Textures/D3D12Texture2D.cpp"
    "src/NoctalEngine/Window/D3D12/D3D12Window.h"
    "src/NoctalEngine/Window/D3D12/D3D12Window.cpp"
    "vendor/ImGui/backends/imgui_impl_dx12.h"
    "vendor/ImGui/backends/imgui_impl_dx12.cpp"
)

# The OpenGL renderer (src/NoctalEngine/Rendering/OpenGL) is left on disk but no longer built:
# the renderer now uses the API-agnostic batch renderer with the D3D12 backend.
# (OpenGLWindow is still built; Window::Create picks the window type.)

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

# Direct3D 12 + DXGI (swap chain) + the runtime HLSL compiler. All ship with the Windows SDK.
target_link_libraries(NoctalEngine PUBLIC d3d12 dxgi d3dcompiler dxguid)

# Keep <windows.h> from defining min/max macros and pulling in rarely used headers.
target_compile_definitions(NoctalEngine PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)

# ImGui's DX12 backend is compiled as part of the engine (the ImGui submodule only builds the OpenGL/SDL3 ones).
# It doesn't use our precompiled header.
set_source_files_properties("vendor/ImGui/backends/imgui_impl_dx12.cpp" PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

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
