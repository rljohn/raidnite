#include "engine/pch.h"
#include "encounter.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

namespace raid
{

Encounter::Encounter()
	: m_StartFrame(0)
	, m_EndFrame(0)
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

void Encounter::Begin(const Frame current)
{
	m_StartFrame = current;
}

void Encounter::End(const Frame current)
{
	m_EndFrame = current;
}

Frame Encounter::GetDuration(const Frame current) const
{
	if (m_StartFrame != 0)
	{
		if (m_EndFrame != 0)
		{
			return (m_EndFrame - m_StartFrame);
		}
		else
		{
			return current - m_StartFrame;
		}
	}
	else
	{
		return 0;
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