#pragma once

#include "engine/types.h"
#include "engine/system/time.h"
#include "engine/system/inlist.h"

#include <iostream>  
using namespace std;

namespace raid
{

enum EncounterEventType
{
	Invalid = 0,
	AbilityStart = 1,
	AbilityEnd = 2,
	HealthChanged = 3,
	ManaChanged = 4,
	AuraGained = 5,
	AuraRefreshed = 6,
	AuraRemoved = 7
};

struct EncounterEventBase
{
	EncounterEventType m_Type = EncounterEventType::Invalid;
	TimeStamp m_Time;
	PlayerId m_Source = InvalidPlayerId;
	PlayerId m_Target = InvalidPlayerId;

	EncounterEventBase();
};

struct EncounterEvent : EncounterEventBase
{
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


class EncounterEvents
{
public:

	static EncounterEvent CreateEvent(PlayerId source, PlayerId target = InvalidPlayerId, const TimeStamp& time = Time::GetCurrent())
	{
		EncounterEvent rvo;
		rvo.m_Source = source;
		rvo.m_Target = target;
		rvo.m_Time = time;
		return rvo;
	}

	static void OnAbilityStart(EncounterEvent& event, SpellId spellId)
	{
		event.m_Type = EncounterEventType::AbilityStart;
		event.m_ExtraData1.Int64 = spellId;
	}
};

} // namespace raid