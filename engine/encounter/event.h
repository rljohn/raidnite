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
	GameStart,
	GameEnd,
	ZoneEnter,
	ZoneExit,
	EncounterStart,
	EncounterEnd,
	EntityCreated,
	EntityDied,
	EntityDestroyed,
	AbilityStart,
	AbilityEnd,
	HealthChanged,
	ManaChanged,
	AuraGained,
	AuraRefreshed,
	AuraRemoved,
};

class Entity;

struct EncounterEvent
{
	EncounterEventType m_Type = EncounterEventType::Invalid;
	Frame m_Frame;
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

	static EncounterEvent CreateEvent(const Frame frame, EventPool* pool, PlayerId source, PlayerId target = InvalidPlayerId)
	{
		EncounterEvent rvo;
		rvo.m_Source = source;
		rvo.m_Target = target;
		rvo.m_Frame = frame;
		return rvo;
	}

	template<EncounterEventType T>
	static EncounterEvent* CreateEvent(const Frame frame, EventPool* pool)
	{
		if (!pool)
			return nullptr;

		EncounterEvent* evt = pool->Create();
		if (evt)
		{
			evt->m_Type = T;
			evt->m_Frame = frame;
		}
		
		return evt;
	}

	static void OnAbilityStart(EncounterEvent& event, SpellId spellId)
	{
		event.m_Type = EncounterEventType::AbilityStart;
		event.m_ExtraData1.Int64 = spellId;
	}

	static void OnEntityCreated(EncounterEvent& event, Entity& entity);
};

} // namespace raid