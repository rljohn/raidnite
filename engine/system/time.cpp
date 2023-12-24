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
	return std::chrono::system_clock::now();
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