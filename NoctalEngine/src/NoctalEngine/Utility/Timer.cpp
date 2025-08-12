#pragma once
#include "Timer.h"

using namespace std::chrono;

namespace NoctalEngine
{
	Timer::Timer()
	{
		m_Last = steady_clock::now();
	}

	float Timer::Mark()
	{
		const auto old = m_Last;
		m_Last = steady_clock::now();
		const duration<float> frameTime = m_Last - old;

		return frameTime.count();
	}
	float Timer::TimeElapsed() const
	{
		return duration<float>(steady_clock::now() - m_Last).count();
	}
}