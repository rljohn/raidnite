#pragma once

#include "engine/types.h"
#include "engine/system/time.h"
#include "engine/system/inlist.h"
#include "engine/system/pool.h"

#include <iostream>  
using namespace std;

namespace raid
{

enum EncounterEventType
{
	Invalid = 0,
	EncounterStart = 1,
	EncounterEnd = 2,
	AbilityStart = 3,
	AbilityEnd = 4,
	HealthChanged = 5,
	ManaChanged = 6,
	AuraGained = 7,
	AuraRefreshed = 8,
	AuraRemoved = 9
};

struct EncounterEvent
{
	EncounterEventType m_Type = EncounterEventType::Invalid;
	TimeStamp m_Time;
	PlayerId m_Source = InvalidPlayerId;
	PlayerId m_Target = InvalidPlayerId;

	union
	{
		int Int;
		double Double;
		float Float;
		int64_t Int64;
	} m_ExtraData1;

	union
	{
		int Int;
		double Double;
		float Float;
		int64_t Int64;
	} m_ExtraData2;

	inlist_node<EncounterEvent> m_Node;

	EncounterEvent();
};


using EventPool = IPool<raid::EncounterEvent>;

class EncounterEvents
{
public:

	static EncounterEvent CreateEvent(EventPool* pool, PlayerId source, PlayerId target = InvalidPlayerId, const TimeStamp& time = Time::GetCurrent())
	{
		EncounterEvent rvo;
		rvo.m_Source = source;
		rvo.m_Target = target;
		rvo.m_Time = time;
		return rvo;
	}

	template<EncounterEventType T>
	static EncounterEvent* CreateEvent(EventPool* pool, const TimeStamp& time = Time::GetCurrent())
	{
		if (!pool)
			return nullptr;

		EncounterEvent* evt = pool->Create();
		if (evt)
		{
			evt->m_Type = T;
			evt->m_Time = time;
		}
		
		return evt;
	}

	static void OnAbilityStart(EncounterEvent& event, SpellId spellId)
	{
		event.m_Type = EncounterEventType::AbilityStart;
		event.m_ExtraData1.Int64 = spellId;
	}
};

} // namespace raid