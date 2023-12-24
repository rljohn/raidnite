#include "engine/pch.h"
#include "encounter.h"

namespace raid
{

Encounter::Encounter()
{
}

void Encounter::Begin(const TimeStamp& current)
{
	m_StartTime = current;
}

void Encounter::End(const TimeStamp& current)
{
	m_EndTime = current;
}

Duration Encounter::GetDuration(const TimeStamp& current) const
{
	if (Time::IsInitialized(m_StartTime))
	{
		if (Time::IsInitialized(m_EndTime))
		{
			return (m_EndTime - m_StartTime);
		}
		else
		{
			return current - m_StartTime;
		}
	}
	else
	{
		return Duration();
	}
}

} // namespace raid