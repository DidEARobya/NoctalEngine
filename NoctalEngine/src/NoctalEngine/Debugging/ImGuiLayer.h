#pragma once
#include "NoctalEngine/Input/Layer.h"
#include "NoctalEngine/Utility/Timer.h"

namespace NoctalEngine
{
	class Window;

	class ImGuiLayer : public Layer
	{
	public:
		struct ScopeTimerResult
		{
			std::string ClassName;
			std::string ScopeTag;
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
		bool m_GroupScopeTimerResults;
	};
}

#ifdef NE_ENABLE_PROFILER
	#define NE_SCOPE_TIMER(className, scopeTag)                                             \
	struct ScopeTimerGuard##__LINE__                                                            \
	{                                                                                           \
		std::string m_ClassName;                                                                \
		std::string m_ScopeTag;                                                                 \
		NoctalEngine::ProfileTimer m_Timer;                                                     \
		ScopeTimerGuard##__LINE__(const char* name, const char* tag)							\
			: m_ClassName(name), m_ScopeTag(tag), m_Timer(m_ScopeTag.c_str()) {}				\
		~ScopeTimerGuard##__LINE__()                                                            \
		{                                                                                       \
			NoctalEngine::ImGuiLayer::ScopeTimerResult result;                                  \
			result.ClassName    = m_ClassName;                                                  \
			result.ScopeTag		= m_ScopeTag;                                                   \
			result.TimeElapsed  = m_Timer.TimeElapsedMs();                                      \
			NoctalEngine::ImGuiLayer::AddScopeTimerResult(result);                              \
		}                                                                                       \
	} timerGuard##__LINE__(className, scopeTag);
#else
	#define NOCTAL_SCOPE_TIMER(profileTag)
#endif
