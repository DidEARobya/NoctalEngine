#pragma once
#include "NoctalEngine/Input/Layer.h"
#include "NoctalEngine/Utility/Timer.h"

namespace NoctalEngine
{
	class ImGuiLayer : public Layer
	{
	public:
		struct ScopeTimerResult
		{
			const char* ScopeTag;
			float TimeElapsed;
		};

		ImGuiLayer();
		virtual ~ImGuiLayer() override;

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float deltaTime) override;

		static void AddScopeTimerResult(const ScopeTimerResult& result) { m_ScopeTimerResults.push_back(result); };
    private:
		void ShowDockSpace(bool* display);

	private:
		static std::vector<ScopeTimerResult> m_ScopeTimerResults;
	};
}

#define NOCTAL_SCOPE_TIMER(profileTag)                                        \
    struct ScopeTimerGuard##__LINE__ {                                        \
        const char* m_Name;                                                   \
        NoctalEngine::ProfileTimer m_Timer;                                   \
        ScopeTimerGuard##__LINE__(const char* name)                           \
            : m_Name(name), m_Timer(name) {}                                  \
        ~ScopeTimerGuard##__LINE__() {                                        \
            NoctalEngine::ImGuiLayer::ScopeTimerResult result;                                          \
            result.ScopeTag = m_Name;                                             \
            result.TimeElapsed = m_Timer.TimeElapsed(); /* use your function */ \
            NoctalEngine::ImGuiLayer::AddScopeTimerResult(result);            \
        }                                                                     \
    } timerGuard##__LINE__(profileTag);