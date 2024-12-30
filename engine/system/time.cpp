#include "engine/pch.h"
#include "time.h"

#include "engine/types.h"
#include "engine/engine.h"

namespace raid {

 /////////////////////
 // Time
 /////////////////////

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

int Time::CountNanosInSeconds(const Nanoseconds& nanos, const Seconds& seconds)
{
    // Convert M seconds to nanoseconds
    Nanoseconds tmp = std::chrono::duration_cast<Nanoseconds>(seconds);
    return (int)(tmp.count() / nanos.count());
}

int Time::CountNanosInMillis(const Nanoseconds& nanos, const Milliseconds& seconds)
{
    // Convert M seconds to nanoseconds
    Nanoseconds tmp = std::chrono::duration_cast<Nanoseconds>(seconds);
    return (int)(tmp.count() / nanos.count());
}

Duration Time::TimeBetween(const TimeStamp& start, const TimeStamp& end)
{
    return end - start;
}

Duration Time::TimeSince(const TimeStamp& t)
{
    return TimeBetween(t, GetCurrent());
}

/////////////////////
// Ticker
/////////////////////

void Ticker::Init(Duration duration, Func cb)
{
    m_Frequency = duration;
    m_TickFunction = cb;
}

void Ticker::Update(const TimeStep& timeStep)
{
    if (m_TickFunction == nullptr)
        return;

    if (Engine* engine = Game::GetEngine())
    {
        m_Accumulation += timeStep;

        while (m_Accumulation >= m_Frequency)
        {
            m_Accumulation -= m_Frequency;
            m_TickFunction();
        }
    }
}

} // namespace raid