#pragma once
#include <chrono>

class Timer {
public:
	Timer();
	float DeltaTime();
	float PeekDelta() const;

private:
	std::chrono::steady_clock::time_point last;
};