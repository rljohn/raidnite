#include "engine/pch.h"
#include "time.h"

using namespace raid;

bool Time::IsInitialized(const TimeStamp& time)
{
	constexpr const TimeStamp empty = TimeStamp();
	return (time != empty);
}

TimeStamp Time::GetCurrent()
{
	return std::chrono::steady_clock::now();
}

Duration Time::FromDeltaTime(float deltaTime)
{
	auto duration_seconds = std::chrono::duration<float>(deltaTime);
	return std::chrono::duration_cast<std::chrono::seconds>(duration_seconds);
}

TimeStamp& Time::Add(TimeStamp& time, const Seconds& seconds)
{
	time += seconds;
	return time;
}

TimeStamp& Time::Add(TimeStamp& time, const Milliseconds& millis)
{
	time += millis;
	return time;
}

Seconds Time::ToSeconds(const Duration& d)
{
	return std::chrono::duration_cast<std::chrono::seconds>(d);
}

Milliseconds Time::ToMillis(const Duration& d)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(d);
}
