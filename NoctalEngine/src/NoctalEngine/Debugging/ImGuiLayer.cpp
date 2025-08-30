#include "nepch.h"
#include "ImGuiLayer.h"
#include "NoctalEngine/Events/Event.h"
#include "NoctalEngine/Events/KeyboardEvents.h"
#include "NoctalEngine/Events/MouseEvents.h"
#include "NoctalEngine/Events/WindowEvents.h"
#include "ImGui/backends/imgui_impl_sdl3.cpp"
#include "NoctalEngine/Input/InputManager.h"
#include "NoctalEngine/Rendering/Renderer.h"
#include <unordered_map>

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
			NOCTAL_SCOPE_TIMER("ImGuiLayer", "Profiler Display");

			std::unordered_map<std::string, std::vector<ScopeTimerResult>> groupedResults;

			for (auto& result : m_ScopeTimerResults)
			{
				groupedResults[result.ClassName].push_back(result);
			}

			for (auto& [header, results] : groupedResults)
			{
				std::sort(results.begin(), results.end(),
					[](const auto& a, const auto& b) 
					{ 
						return a.TimeElapsed > b.TimeElapsed; 
					});

				if (ImGui::CollapsingHeader(header.c_str()))
				{
					for (auto& result : results)
					{
						ImGui::Text("Process %s took %.3fms to complete", result.ScopeTag.c_str(), result.TimeElapsed);
					}
				}
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

