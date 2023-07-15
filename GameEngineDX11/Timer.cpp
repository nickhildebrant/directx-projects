#include "Timer.h"

Timer::Timer()
{
	lastTimePoint = std::chrono::steady_clock::now();
}

float Timer::DeltaTime()
{
	const auto oldTime = lastTimePoint;
	lastTimePoint = std::chrono::steady_clock::now();

	const std::chrono::duration<float> frameTime = lastTimePoint - oldTime;

	return frameTime.count();
}

float Timer::PeekDelta() const
{
	return std::chrono::duration<float>( std::chrono::steady_clock::now() - lastTimePoint ).count();
}