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
	static Seconds ToSeconds(const Duration& d);
	static Milliseconds ToMillis(const Duration& d);

	// Printing
	using TimeDisplay = char[32];
	static void GetHMS(const Duration& d, TimeDisplay& buffer);
};

} // namespace raid