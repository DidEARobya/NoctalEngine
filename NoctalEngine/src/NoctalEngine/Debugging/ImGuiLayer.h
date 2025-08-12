#pragma once
#include "NoctalEngine/Input/Layer.h"

namespace NoctalEngine
{
	class NOCTAL_ENGINE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer() override;

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float deltaTime) override;
		void OnEvent(NoctalEngine::Event& event) override;
	};
}