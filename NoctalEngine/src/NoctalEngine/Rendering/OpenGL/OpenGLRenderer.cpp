#include "nepch.h"
#include "OpenGLRenderer.h"
#include "NoctalEngine/Window/Window.h"
#include "NoctalEngine/Application/Application.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl3.h"
#include "glm/glm.hpp"
#include "SDL3/SDL.h"
#include "NoctalEngine/Rendering/OpenGL/OpenGLVertexBuffer.h"
#include "NoctalEngine/Rendering/OpenGL/OpenGLIndexBuffer.h"

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

	float vertices[3 * 3]
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	//m_Bindables.push_back(std::make_unique<OpenGLVertexBuffer>(vertices));

	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);

	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	unsigned int indices[3] = { 0, 1, 2 };

	//m_Bindables.push_back(std::make_unique<OpenGLIndexBuffer>(indices));
	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	std::string vertexSource = R"(
		#version 330 core

		layout(location = 0) in vec3 a_Position;
		
		out vec3 v_Position;

		void main()
		{
			v_Position = a_Position;
			gl_Position = vec4(a_Position, 1.0);
		}	

	)";

	std::string pixelSource = R"(
		#version 330 core

		layout(location = 0) out vec4 colour;

		in vec3 v_Position;

		void main()
		{
			colour = vec4(v_Position * 0.5 + 0.5, 1.0);
		}	

	)";

	m_Bindables.push_back(std::make_unique<OpenGLShader>(vertexSource, pixelSource));
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

	glClearColor(0.1f, 0.1f, 0.1f, 0);
}

void OpenGLRenderer::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto& bindable : m_Bindables)
	{
		bindable->Bind();
	}

	glBindVertexArray(m_VertexArray);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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