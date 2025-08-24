#pragma once
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

		bool start = true;
		ImGui::ShowDemoWindow(&start);

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
	}
}

