#include "Timer.h"

Timer::Timer()
{
	lastTimePoint = std::chrono::steady_clock::now();
}

float Timer::DeltaTime()
{
	const auto oldTimePoint = lastTimePoint;
	lastTimePoint = std::chrono::steady_clock::now();

	const std::chrono::duration<float> frameTime = lastTimePoint - oldTimePoint;

	return frameTime.count();
}

float Timer::PeekDelta() const
{
	return std::chrono::duration<float>( std::chrono::steady_clock::now() - lastTimePoint ).count();
}