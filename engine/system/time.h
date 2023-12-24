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

	// Add Helpers
	static TimeStamp& Add(TimeStamp& time, const Seconds& seconds);
	static TimeStamp& Add(TimeStamp& time, const Milliseconds& millis);
	
	// Conversation
	static Seconds ToSeconds(const Duration& d);
	static Milliseconds ToMillis(const Duration& d);
};

} // namespace raid