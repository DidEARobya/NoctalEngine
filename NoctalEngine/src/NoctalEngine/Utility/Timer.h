#pragma once
#include <chrono>

namespace NoctalEngine
{
	class Timer
	{
	public:
		Timer();
		float Mark();
		float TimeElapsed() const;

	protected:
		std::chrono::steady_clock::time_point m_Last;
	};

	class ProfileTimer : public Timer
	{
	public:
		ProfileTimer(const std::string& tag) : Timer(), m_Tag(tag) {}
		~ProfileTimer()
		{
			NE_ENGINE_INFO("Process '{}' took {}ms to complete", m_Tag, TimeElapsed());
		}

	private:
		std::string m_Tag;
	};
}