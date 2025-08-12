#pragma once
#include "NoctalEngine/Input/Layer.h"

namespace NoctalEngine
{
	class MouseButtonPressedEvent;
	class MouseButtonReleasedEvent;
	class MouseMovedEvent;
	class MouseScrollEvent;
	class KeyPressedEvent;
	class KeyReleasedEvent;
	class KeyTypedEvent;
	class WindowResizeEvent;

	class NOCTAL_ENGINE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer() override;

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float deltaTime) override;
		void OnEvent(Event& event) override;

	private:
		bool OnMouseButtonPressedEvent(const MouseButtonPressedEvent& event);
		bool OnMouseButtonReleasedEvent(const MouseButtonReleasedEvent& event);
		bool OnMouseMovedEvent(const MouseMovedEvent& event);
		bool OnMouseScrollEvent(const MouseScrollEvent& event);
		bool OnKeyPressedEvent(const KeyPressedEvent& event);
		bool OnKeyReleasedEvent(const KeyReleasedEvent& event);
		bool OnKeyTypedEvent(const KeyTypedEvent& event);
		bool OnWindowResizeEvent(const WindowResizeEvent& event);
	};
}