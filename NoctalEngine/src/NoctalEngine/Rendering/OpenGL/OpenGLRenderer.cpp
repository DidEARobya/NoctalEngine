#include "nepch.h"
#include "OpenGLRenderer.h"
#include "NoctalEngine/Window/Window.h"
#include "NoctalEngine/Application/Application.h"
#include "Buffers/OpenGLIndexBuffer.h"
#include "Shaders/OpenGLVertexShader.h"
#include "Shaders/OpenGLFragmentShader.h"
#include "NoctalEngine/Rendering/Shaders/Shader.h"
#include "Drawables/OpenGLDrawable.h"
#include "Textures/OpenGLTexture2D.h"
#include "NoctalEngine/Rendering/Buffers/BufferLayout.h"
#include "NoctalEngine/Rendering/Textures/Texture.h"
#include "NoctalEngine/Rendering/OpenGL/Drawables/OpenGLQuad.h"
#include "NoctalEngine/Rendering/OpenGL/Drawables/OpenGLTriangle.h"

#include <ImGui/backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>
#include <GLAD/glad.h>
#include <filesystem>

struct RendererData
{
	const uint32_t MaxQuads = 10000;
	const uint32_t MaxVerts = MaxQuads * 4;
	const uint32_t MaxIndices = MaxQuads * 6;
};

static RendererData s_Data;

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
	m_ShaderLibrary->LoadShader(ASSET_DIR "Shaders/OpenGL/ColouredTextureVS.vert", false);
	m_ShaderLibrary->LoadShader(ASSET_DIR "Shaders/OpenGL/ColouredTextureFS.frag", false);

	m_ShaderLibrary->SortShaders();

	m_FrameBuffer = std::make_unique<OpenGLFrameUniformBufferObject>();
	//m_ObjectBuffer = std::make_shared<OpenGLObjectUniformBufferObject>();
}

void OpenGLRenderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}

void OpenGLRenderer::BeginRender()
{
	NE_SCOPE_TIMER("Renderer::BeginRender", "BeginFrame");

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
	NE_SCOPE_TIMER("Renderer::Render", "Render");
	glClear(GL_COLOR_BUFFER_BIT);

	{
		NE_SCOPE_TIMER("Renderer::Render", "DrawDrawables");
		int32_t index = 0;

		for (auto& drawable : m_Drawables)
		{
			drawable.get()->Draw();
			index++;
		}
	}

	ImGui::Render();
}

void OpenGLRenderer::EndRender()
{
	NE_SCOPE_TIMER("Renderer::EndFrame", "EndFrame");
	{
		NE_SCOPE_TIMER("Renderer::EndFrame", "FrameBufferUpdate");
		m_FrameData.ViewProjection = NoctalEngine::Application::Get().GetCameraViewProjection();
		m_FrameBuffer->UpdateFrameData(m_FrameData);
		m_FrameBuffer->Bind();
	}
	{
		NE_SCOPE_TIMER("Renderer::EndFrame", "ImGuiEndFrame");
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
	}

	{
		NE_SCOPE_TIMER("Renderer::EndFrame", "SwapWindow");
		NE_ENGINE_SDL_ASSERT(SDL_GL_SwapWindow(m_Window));
	}
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

std::shared_ptr<Drawable> OpenGLRenderer::CreateDrawable(NoctalEngine::Geometry type, glm::vec2 position, glm::vec2 scale)
{
	std::shared_ptr<Drawable> drawable;

	switch (type)
	{
		case NoctalEngine::Geometry::QUAD:
			drawable = std::make_shared<OpenGLQuad>(position, scale);
			break;
		
		case NoctalEngine::Geometry::TRIANGLE:
			drawable = std::make_shared<OpenGLTriangle>(position, scale);
			break;

		default:
			NE_ENGINE_ERROR("Geometry type is invalid");
			return nullptr;
	}
 
	if (drawable == nullptr)
	{
		NE_ENGINE_ERROR("Failed to create Drawable");
		return nullptr;
	}

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
