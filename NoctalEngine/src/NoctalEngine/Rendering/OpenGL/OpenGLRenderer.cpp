#include "nepch.h"
#include "OpenGLRenderer.h"
#include "NoctalEngine/Window/Window.h"
#include "NoctalEngine/Application/Application.h"
#include "OpenGLIndexBuffer.h"
#include "OpenGLShader.h"
#include "OpenGLVertexBuffer.h"
#include "Drawables/OpenGLDrawable.h"

#include <ImGui/backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>
#include <glm/glm.hpp>
#include <GLAD/include/glad/glad.h>

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
}

void OpenGLRenderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}

void OpenGLRenderer::BeginRender(const glm::mat4& camera)
{
	m_CameraViewProjectionMatrix = camera;

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
		drawable->Draw();
	}

	ImGui::Render();
}

void OpenGLRenderer::EndRender()
{
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

NoctalEngine::Shader* OpenGLRenderer::CreateShader(const std::string& vertexSource, const std::string& pixelSource)
{
	return new OpenGLShader(vertexSource, pixelSource);
}

NoctalEngine::VertexBuffer* OpenGLRenderer::CreateVertexBuffer(float* vertices, uint32_t size, const NoctalEngine::BufferLayout& layout)
{
	return new OpenGLVertexBuffer(vertices, size, layout);
}

NoctalEngine::IndexBuffer* OpenGLRenderer::CreateIndexBuffer(uint32_t* indices, uint32_t size)
{
	return new OpenGLIndexBuffer(indices, size);
}

void OpenGLRenderer::CreateDrawable(NoctalEngine::Geometry geometry)
{
	m_Drawables.push_back(std::make_unique<OpenGLDrawable>(geometry));
}

void OpenGLRenderer::DrawIndexed()
{
	glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
}
