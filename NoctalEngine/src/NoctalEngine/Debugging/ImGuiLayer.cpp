#include "nepch.h"
#include "ImGuiLayer.h"
#include "NoctalEngine/Events/Event.h"
#include "NoctalEngine/Events/KeyboardEvents.h"
#include "NoctalEngine/Events/MouseEvents.h"
#include "NoctalEngine/Events/WindowEvents.h"
#include "NoctalEngine/Window/Window.h"
#include "ImGui/backends/imgui_impl_sdl3.cpp"
#include "NoctalEngine/Input/InputManager.h"
#include "NoctalEngine/Rendering/Renderer.h"
#include <map>

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
			NE_SCOPE_TIMER("ImGuiLayer", "Profiler Display");

			ImGui::Checkbox("Group Results", &m_GroupScopeTimerResults);

			std::map<std::string, std::vector<ScopeTimerResult>> groupedResults;

			for (auto& result : m_ScopeTimerResults)
			{
				auto& vec = groupedResults[result.ClassName];

				if (m_GroupScopeTimerResults == false)
				{
					vec.push_back(result);
					continue;
				}

				// Check if a result with the same ScopeTag already exists
				auto it = std::find_if(vec.begin(), vec.end(),
					[&result](const ScopeTimerResult& existing) {
						return existing.ScopeTag == result.ScopeTag;
					});

				if (it != vec.end())
				{
					// If found, accumulate the time
					it->TimeElapsed += result.TimeElapsed;
				}
				else
				{
					// Otherwise, add as new
					vec.push_back(result);
				}
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
		ImGuiWindowFlags windowFlags = 
			ImGuiWindowFlags_NoDocking			    |
			ImGuiWindowFlags_NoTitleBar				|
			ImGuiWindowFlags_NoCollapse				|
			ImGuiWindowFlags_NoResize				|
			ImGuiWindowFlags_NoMove					|
			ImGuiWindowFlags_NoBringToFrontOnFocus  |
			ImGuiWindowFlags_NoBackground           |
			ImGuiWindowFlags_NoNavFocus;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace", display, windowFlags);
		ImGui::PopStyleVar(3);

		ImGuiID id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

		ImGui::End();
	}
}

