#include "nepch.h"
#include "Timer.h"

namespace NoctalEngine
{
	Timer::Timer()
	{
		m_Last = std::chrono::steady_clock::now();
	}

	float Timer::Mark()
	{
		const auto old = m_Last;
		m_Last = std::chrono::steady_clock::now();
		const std::chrono::duration<float> frameTime = m_Last - old;

		return frameTime.count();
	}
	float Timer::TimeElapsed() const
	{
		return std::chrono::duration<float>(std::chrono::steady_clock::now() - m_Last).count();
	}
	float Timer::TimeElapsedMs() const
	{
		return std::chrono::duration<float, std::milli>(
			std::chrono::steady_clock::now() - m_Last
		).count();
	}
}