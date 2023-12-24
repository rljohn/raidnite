#pragma once

#include "engine/types.h"
#include "engine/system/time.h"

namespace raid
{

class Encounter
{

public:

	Encounter();
	
	void Begin(const TimeStamp& current = Time::GetCurrent());
	void End(const TimeStamp& current = Time::GetCurrent());

	Duration GetDuration(const TimeStamp& current = Time::GetCurrent()) const;

private:

	TimeStamp m_StartTime;
	TimeStamp m_EndTime;
};

} // namespace raid
