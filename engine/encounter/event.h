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
	OccupancyChanged,
	PositionChanged,

	MAX
};

class Entity;

union EncounterField
{
	int Int;
	double Double;
	float Float;
	int64_t Int64;
	uint8_t Buffer[8];
};

struct EncounterEvent
{
	EncounterEventType m_Type = EncounterEventType::Invalid;
	Frame m_Frame;
	EntityId m_Source = InvalidEntityId;
	EntityId m_Target = InvalidEntityId;

	EncounterField m_ExtraData1;
	EncounterField m_ExtraData2;

	EncounterEvent();

	inlist_node<EncounterEvent> m_Node;
};


using EventPool = IPool<raid::EncounterEvent>;

class EncounterEvents
{
public:

	template<EncounterEventType T>
	static EncounterEvent* Create(const Frame frame, EventPool* pool)
	{
		if (!pool)
			return nullptr;

		EncounterEvent* evt = pool->Create();
		if (evt)
		{
			evt->m_Type = T;
			evt->m_Frame = frame;
			evt->m_Source = InvalidEntityId;
			evt->m_Target = InvalidEntityId;
			memset(&evt->m_ExtraData1, 0, sizeof(evt->m_ExtraData1));
			memset(&evt->m_ExtraData2, 0, sizeof(evt->m_ExtraData2));
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