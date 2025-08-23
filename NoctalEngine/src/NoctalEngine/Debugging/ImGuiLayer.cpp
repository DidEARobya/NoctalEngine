#pragma once
#include "nepch.h"
#include "ImGuiLayer.h"
#include "NoctalEngine/Events/Event.h"
#include "NoctalEngine/Events/KeyboardEvents.h"
#include "NoctalEngine/Events/MouseEvents.h"
#include "NoctalEngine/Events/WindowEvents.h"
#include "ImGui/backends/imgui_impl_sdl3.cpp"
#include "NoctalEngine/Input/InputManager.h"
#include "NoctalEngine/Input/KeyCodes.h"

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

		if (ImGui::Begin("Mouse Position"))
		{
			float mouseX;
			float mouseY;
			InputManager::GetMousePosition(mouseX, mouseY);

			ImGui::Text("Mouse Position %.0f X : %.0f Y", mouseX, mouseY);
		}

		ImGui::End();	
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(NOCTAL_BIND_EVENT_FN(OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(NOCTAL_BIND_EVENT_FN(OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(NOCTAL_BIND_EVENT_FN(OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrollEvent>(NOCTAL_BIND_EVENT_FN(OnMouseScrollEvent));
		dispatcher.Dispatch<KeyPressedEvent>(NOCTAL_BIND_EVENT_FN(OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(NOCTAL_BIND_EVENT_FN(OnKeyReleasedEvent));
		// Char reading from custom polling has to be fixed, ImGui SDL event function used for char events, for now.
		//dispatcher.Dispatch<KeyTypedEvent>(NOCTAL_BIND_EVENT_FN(OnKeyTypedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(NOCTAL_BIND_EVENT_FN(OnWindowResizeEvent));
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(const MouseButtonPressedEvent& event)
	{
		NE_ENGINE_INFO(event.GetMouseButton());

		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent(event.GetMouseButton() - 1, true);

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(const MouseButtonReleasedEvent& event)
	{
		NE_ENGINE_INFO(event.GetMouseButton());

		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent(event.GetMouseButton() - 1, false);
		
		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(const MouseMovedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(event.GetMouseXPos(), event.GetMouseYPos());

		return false;
	}

	bool ImGuiLayer::OnMouseScrollEvent(const MouseScrollEvent& event)
	{
		NE_ENGINE_INFO("Mouse Scroll Event");

		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseWheelEvent(event.GetXOffset(), event.GetYOffset());

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(const KeyPressedEvent& event)
	{
		ImGuiKey key = ImGui_ImplSDL3_KeyEventToImGuiKey(event.GetKeyCode(), (SDL_Scancode)event.GetScanCode());

		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent(key, true);

		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(const KeyReleasedEvent& event)
	{
		ImGuiKey key = ImGui_ImplSDL3_KeyEventToImGuiKey(event.GetKeyCode(), (SDL_Scancode)event.GetScanCode());

		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent(key, false);

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(const KeyTypedEvent& event)
	{
		ImGuiKey keyCode = ImGui_ImplSDL3_KeyEventToImGuiKey(event.GetKeyCode(), (SDL_Scancode)event.GetScanCode());
		
		ImGuiIO& io = ImGui::GetIO();

			io.AddInputCharacter(keyCode);

		return false;
	}
	bool ImGuiLayer::OnWindowResizeEvent(const WindowResizeEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(event.GetWidth(), event.GetHeight());
		io.DisplayFramebufferScale = ImVec2(event.GetWidth(), event.GetHeight());

		return false;
	}
}

