#pragma once
#include "NoctalEngine/Core.h"
#include "NoctalEngine/Input/LayerStack.h"
#include "NoctalEngine/Utility/Timer.h"
#include "NoctalEngine/Rendering/Camera/OrthographicCamera.h"

namespace NoctalEngine
{
	class Window;
	class Event;
	class WindowClosedEvent;
	class Layer;
	class ImGuiLayer;
	class AppLayer;

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		bool CloseApplication(const WindowClosedEvent& closeEvent);

		virtual const glm::mat4& GetCameraViewProjection() const = 0;

		inline static Application& Get() { return *s_Instance; };
		inline Window& GetWindow() { return *m_Window; };

		static float DeltaTime() { return s_Instance->m_DeltaTime; };

	protected:
		AppLayer* m_GameLayer = nullptr;

	private:
		Window* m_Window = nullptr;
		ImGuiLayer* m_ImGuiLayer = nullptr;

		Timer m_Timer;

		bool m_AppRunning = true;

		LayerStack m_LayerStack;
		inline static Application* s_Instance;

		float m_DeltaTime;
	};

	Application* CreateApplication();
}