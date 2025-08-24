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

	private:
		std::chrono::steady_clock::time_point m_Last;
	};
}