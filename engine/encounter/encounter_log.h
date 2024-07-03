#pragma once

#include "engine/game/combat.h"
#include "engine/game/game.h"

#include "engine/system/inlist.h"
#include "engine/system/pool.h"
#include "engine/encounter/event.h"
#include "engine/system/log/log_channels.h"

namespace raid
{

class Encounter;

class IEncounterLog
{
public:

	IEncounterLog();

	virtual const std::vector<Encounter*>& GetEncounterList() const = 0;
	
	using DisplayString = char[128];
	virtual void GetDisplayString(const Encounter& encounter, DisplayString& buffer) const = 0;

	virtual void Clear() = 0;

protected:

	Frame m_StartFrame;
};

class EncounterLog : public IEncounterLog
{
	friend class EncounterSerialization;

public:

	EncounterLog();

	Encounter* CreateEncounter();

	bool Init(EventPool* pool = nullptr);
	void Shutdown();
	void Clear() override;

	void OnGameEvent(const GameEvent* evt);

	const std::vector<Encounter*>& GetEncounterList() const override
	{ 
		return m_Encounters; 
	}


	Milliseconds GetTimeSince(const Frame frame, const Frame since) const;
	Milliseconds GetTimeSince(const Frame frame) const 
	{ 
		return GetTimeSince(frame, m_StartFrame); 
	}
	
	void GetDisplayString(const Encounter& encounter, DisplayString& buffer) const override;

private:

	void OnGameStart();
	void OnGameEnd();
	void OnZoneEnter();
	void OnZoneExit();
	void OnCombatStart();
	void OnCombatEnd();
	void OnEntityDied(const DeathEvent* deathDevent);
	void OnDamageEvent(const DamageEvent* damageEvent);

	template <EncounterEventType T>
	EncounterEvent* CreateEvent()
	{
		return EncounterEvents::CreateEvent<T>(GetFrame(), m_EventPool);
	}

	bool AddEvent(EncounterEvent* evt);

	template <EncounterEventType T>
	EncounterEvent* AddEvent()
	{
		EncounterEvent* evt = nullptr;
		if (evt = CreateEvent<T>())
		{
			if (!AddEvent(evt))
			{
				delete evt;
				evt = nullptr;
			}
		}

		return evt;
	}

	EncounterEvent* LoadEvent()
	{
		if (m_EventPool)
		{
			return m_EventPool->Create();
		}

		return nullptr;
	}

	Frame GetFrame() const;

	Encounter* m_ActiveEncounter;
	EventPool* m_EventPool;

	GameEventDelegate::Function m_OnGameEvent;
	std::vector<Encounter*> m_Encounters;
	Frame m_StartFrame;
};

}