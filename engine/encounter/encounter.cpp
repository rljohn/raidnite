#include "engine/pch.h"
#include "encounter.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

namespace raid
{

Encounter::Encounter()
{
	m_Name[0] = '\0';
}

void Encounter::Shutdown(EventPool* pool)
{
	while (!m_Events.empty())
	{
		auto* node = m_Events.front();
		m_Events.pop_front();
		pool->Free(node->data);
	}
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

void Encounter::AddEvent(EncounterEvent* evt)
{
	m_Events.push_back(evt->m_Node);
}

void Encounter::UpdateName()
{

}

} // namespace raid