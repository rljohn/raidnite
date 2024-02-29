#pragma once

#include "engine/game/combat.h"
#include "engine/game/game.h"

#include "engine/system/inlist.h"
#include "engine/system/pool.h"
#include "engine/encounter/event.h"

namespace raid
{

class Encounter;

class IEncounterLog
{

};

class EncounterLog : public IEncounterLog
{
	friend class EncounterSerialization;

public:

	EncounterLog();

	Encounter* CreateEncounter();

	bool Init(EventPool* pool = nullptr);
	void Shutdown();
	void Clear();

	void OnGameEvent(const GameEvent* evt);

	const std::list<Encounter*>& GetEncounterList() const 
	{ 
		return m_Encounters; 
	}

private:

	void OnZoneEnter();
	void OnZoneExit();
	void OnCombatStart();
	void OnCombatEnd();
	void OnEntityDied(const DeathEvent* deathDevent);
	void OnDamageEvent(const DamageEvent* damageEvent);

	template <EncounterEventType T>
	EncounterEvent* CreateEvent()
	{
		return EncounterEvents::CreateEvent<T>(m_EventPool);
	}

	EncounterEvent* LoadEvent()
	{
		if (m_EventPool)
		{
			return m_EventPool->Create();
		}

		return nullptr;
	}

	Encounter* m_ActiveEncounter;
	EventPool* m_EventPool;

	GameEventDelegate::Function m_OnGameEvent;
	std::list<Encounter*> m_Encounters;
};

}