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
		//void OnEvent(Event& event) override;
	};
}