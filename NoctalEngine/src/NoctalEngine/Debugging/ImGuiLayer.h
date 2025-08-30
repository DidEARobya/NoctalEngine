#pragma once
#include "NoctalEngine/Input/Layer.h"

namespace NoctalEngine
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer() override;

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float deltaTime) override;

    private:
		void ShowDockSpace(bool* display);

		//void OnEvent(Event& event) override;
	};
}