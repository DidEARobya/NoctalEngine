#include "nepch.h"
#include "ImGuiLayer.h"
#include "NoctalEngine/Events/Event.h"
#include "NoctalEngine/Events/KeyboardEvents.h"
#include "NoctalEngine/Events/MouseEvents.h"
#include "NoctalEngine/Events/WindowEvents.h"
#include "ImGui/backends/imgui_impl_sdl3.cpp"
#include "NoctalEngine/Input/InputManager.h"
#include "NoctalEngine/Rendering/Renderer.h"

namespace NoctalEngine
{
	std::vector<ImGuiLayer::ScopeTimerResult> ImGuiLayer::m_ScopeTimerResults;

	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ImGuiIO& io = ImGui::GetIO();
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnUpdate(float deltaTime)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = deltaTime;

		bool dockDisplay = true;
		ShowDockSpace(&dockDisplay);

		if (ImGui::Begin("Simulation Speed"))
		{
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		ImGui::End();

		if (ImGui::Begin("Renderer"))
		{
			ImGui::Text("Vendor: %s", Renderer::Instance().GetVendor());
			ImGui::Text("Renderer: %s", Renderer::Instance().GetRenderer());
			ImGui::Text("Version: %s", Renderer::Instance().GetVersion());
		}

		ImGui::End();	

		if (ImGui::Begin("Profiler"))
		{
			std::sort(
				m_ScopeTimerResults.begin(),
				m_ScopeTimerResults.end(),
				[](const NoctalEngine::ImGuiLayer::ScopeTimerResult& a,
					const NoctalEngine::ImGuiLayer::ScopeTimerResult& b)
				{
					return a.TimeElapsed > b.TimeElapsed; // Descending
				});

			for (auto& result : m_ScopeTimerResults)
			{
				ImGui::Text("Process %s took %.3fms to complete", (const char*)result.ScopeTag.c_str(), result.TimeElapsed);
			}

			m_ScopeTimerResults.clear();
		}

		ImGui::End();
	}
	void ImGuiLayer::ShowDockSpace(bool* display)
	{
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		// This removes background/border so it's "invisible"
		windowFlags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("InvisibleDockSpace", display, windowFlags);
		ImGui::PopStyleVar(3);

		// DockSpace ID: needs to be consistent across frames
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f),
			ImGuiDockNodeFlags_PassthruCentralNode);

		ImGui::End();
	}
}

