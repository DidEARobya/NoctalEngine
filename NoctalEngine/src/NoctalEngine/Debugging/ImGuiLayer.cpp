#pragma once
#include "nepch.h"
#include "ImGuiLayer.h"
#include "NoctalEngine/Events/Event.h"

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

	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnUpdate(float deltaTime)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = deltaTime;

		if (ImGui::Begin("Simulation Speed"))
		{
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		ImGui::End();	
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
	}
}

