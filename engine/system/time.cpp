#include "engine/pch.h"
#include "time.h"
#include "engine/types.h"

namespace raid {

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

void Time::GetHMS(const Duration& d, TimeDisplay& buffer)
{
    using namespace std::chrono;

    Duration ns = d;

    // Convert nanoseconds to seconds
    std::chrono::seconds secs = duration_cast<seconds>(d);
    ns -= duration_cast<nanoseconds>(secs);

    // Convert seconds to minutes
    std::chrono::minutes mins = duration_cast<minutes>(secs);
    secs -= duration_cast<seconds>(mins);

    // Convert minutes to hours
    std::chrono::hours hrs = duration_cast<hours>(mins);
    mins -= duration_cast<minutes>(hrs);

    // If there are hours, format HH:MM:SS
    if (hrs.count() > 0)
    {
        std::snprintf(buffer, COUNTOF(buffer), "%02d:%02d:%02lld", hrs.count(), mins.count(), secs.count());
    }
    else
    { 
        // Otherwise, format MM:SS
        std::snprintf(buffer, COUNTOF(buffer), "%02d:%02lld", mins.count(), secs.count());
    }
}

} // namespace raid