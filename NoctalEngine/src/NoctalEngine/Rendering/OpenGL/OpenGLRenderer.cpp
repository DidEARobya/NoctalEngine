#include "nepch.h"
#include "OpenGLRenderer.h"
#include "NoctalEngine/Window/Window.h"
#include "NoctalEngine/Application/Application.h"
#include "Buffers/OpenGLIndexBuffer.h"
#include "Shaders/OpenGLVertexShader.h"
#include "Shaders/OpenGLFragmentShader.h"
#include "NoctalEngine/Rendering/Shaders/Shader.h"
#include "Buffers/OpenGLVertexBuffer.h"
#include "Drawables/OpenGLDrawable.h"
#include "Textures/OpenGLTexture2D.h"
#include "NoctalEngine/Rendering/Buffers/VertexBuffer.h"
#include "NoctalEngine/Rendering/Buffers/IndexBuffer.h"
#include "NoctalEngine/Rendering/Buffers/BufferLayout.h"
#include "NoctalEngine/Rendering/Textures/Texture.h"

#include <ImGui/backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>
#include <glm/glm.hpp>
#include <GLAD/include/glad/glad.h>
#include <filesystem>

void OpenGLRenderer::Init(const NoctalEngine::Window* windowRef)
{
	m_Window = windowRef->GetSDLWindow();
	NE_ENGINE_SDL_ASSERT(m_Window);

	m_GLContext = SDL_GL_CreateContext(m_Window);
	NE_ENGINE_SDL_ASSERT(m_GLContext);

	NE_ENGINE_INFO("SDL successfully loaded OpenGL_Context");

	int status = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
	NE_ENGINE_ASSERT(status, "GLAD failed to initialise");
	NE_ENGINE_INFO("GLAD successfully loaded");

	NE_ENGINE_SDL_ASSERT(SDL_GL_SetSwapInterval(1));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0, 0, windowRef->GetWidth(), windowRef->GetHeight());

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui::StyleColorsDark();

	ImGui_ImplOpenGL3_Init();
	ImGui_ImplSDL3_InitForOpenGL(windowRef->GetSDLWindow(), m_GLContext);

	m_ShaderLibrary = std::make_unique<NoctalEngine::ShaderLibrary>();

	//Load Engine Shaders, false to avoid sorting after each new load
	m_ShaderLibrary->LoadShader(ASSET_DIR "Shaders/OpenGL/TextureVS.vert", false);
	m_ShaderLibrary->LoadShader(ASSET_DIR "Shaders/OpenGL/TextureFS.frag", false);
	m_ShaderLibrary->LoadShader(ASSET_DIR "Shaders/OpenGL/SolidColourVS.vert", false);
	m_ShaderLibrary->LoadShader(ASSET_DIR "Shaders/OpenGL/SolidColourFS.frag", false);

	m_ShaderLibrary->SortShaders();
}

void OpenGLRenderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}

void OpenGLRenderer::BeginRender()
{
	ImGuiIO& io = ImGui::GetIO();
	NoctalEngine::Application& app = NoctalEngine::Application::Get();
	io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
}

void OpenGLRenderer::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto& drawable : m_Drawables)
	{
		drawable.get()->Draw();
	}

	ImGui::Render();
}

void OpenGLRenderer::EndRender()
{
	m_CameraViewProjectionMatrix = NoctalEngine::Application::Get().GetCameraViewProjection();

	ImGuiIO& io = ImGui::GetIO();

	ImGui::EndFrame();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* window = SDL_GL_GetCurrentWindow();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(window, m_GLContext);
	}

	NE_ENGINE_SDL_ASSERT(SDL_GL_SwapWindow(m_Window));
}

void OpenGLRenderer::OnWindowResize(const uint32_t width, const uint32_t height)
{
	glViewport(0, 0, width, height);
}

const char* OpenGLRenderer::GetVendor()
{
	return (const char*)glGetString(GL_VENDOR);
}

const char* OpenGLRenderer::GetRenderer()
{
	return (const char*)glGetString(GL_RENDERER);
}

const char* OpenGLRenderer::GetVersion()
{
	return (const char*)glGetString(GL_VERSION);
}

std::shared_ptr<NoctalEngine::Shader> OpenGLRenderer::GetShader(const std::string& shaderName)
{
	NE_ENGINE_ASSERT(m_ShaderLibrary, "ShaderLibrary doesn't exist");

	return m_ShaderLibrary->GetShader(shaderName);
}

std::shared_ptr<NoctalEngine::Shader> OpenGLRenderer::CreateShader(const std::string& filePath)
{
	std::filesystem::path path(filePath);
	std::string type = path.extension().string();

	if (type == ".vert")
	{
		return std::make_shared<OpenGLVertexShader>(filePath);
	}
	else if (type == ".frag")
	{
		return std::make_shared<OpenGLFragmentShader>(filePath);
	}

	NE_ENGINE_ASSERT(false, "{} shader type not supported", type);
	return nullptr;
}

std::shared_ptr<Drawable> OpenGLRenderer::CreateDrawable(NoctalEngine::Geometry geometry)
{
	std::shared_ptr<Drawable> drawable = std::make_shared<OpenGLDrawable>(geometry);
	m_Drawables.push_back(drawable);

	return drawable;
}

std::shared_ptr<NoctalEngine::Texture> OpenGLRenderer::CreateTexture(const std::string& path)
{
	return std::make_shared<OpenGLTexture2D>(path);
}

void OpenGLRenderer::DrawIndexed()
{
	glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
}
