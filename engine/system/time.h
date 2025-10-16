#pragma once

#include "engine/types.h"

namespace raid
{

class Time
{
public:

	static bool IsInitialized(const TimeStamp& time);

	// Time Helpers
	static TimeStamp GetCurrent();

	// Unreal Helper (deltaTime is seconds)
	static Duration FromDeltaTime(float deltaTime);

	// Add Helpers
	static TimeStamp& Add(TimeStamp& time, const Seconds& seconds);
	static TimeStamp& Add(TimeStamp& time, const Milliseconds& millis);
	
	// Conversation
	static Duration ToNanoSeconds(double seconds);
	static Seconds ToSeconds(const Duration& d);
	static Milliseconds ToMillis(const Duration& d);

	// Printing
	using TimeDisplay = char[32];
	static void GetHMS(const Duration& d, TimeDisplay& buffer);

	// Math
	static int CountNanosInSeconds(const Nanoseconds& nanos, const Seconds& seconds);
	static int CountNanosInMillis(const Nanoseconds& nanos, const Milliseconds& seconds);

	// Delta
	static Duration TimeBetween(const TimeStamp& start, const TimeStamp& end);
	static Duration TimeSince(const TimeStamp& start);
};

// Helper Class - Ticks at a set interval
struct Ticker
{
	using Func = std::function<void()>;
	
	void Init(Duration duration, Func cb);
	void Update(const TimeStep& timeStep);

private:

	Func m_TickFunction = nullptr;
	Duration m_Accumulation = Duration(0);
	Duration m_Frequency = Duration(0);
};

} // namespace raid