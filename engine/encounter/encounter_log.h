#pragma once

#include "engine/game/combat.h"
#include "engine/game/game.h"

#include "engine/system/inlist.h"
#include "engine/system/pool.h"
#include "engine/encounter/event.h"
#include "engine/encounter/encounter_data.h"
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
	virtual void GetDisplayString(const EncounterEvent& e, DisplayString& buffer) const = 0;

	virtual void Clear() = 0;

protected:

	Frame m_StartFrame;
};

class EncounterLog : public IEncounterLog, public IGameEventListener
{
	friend class EncounterSerialization;

public:

	EncounterLog();
	~EncounterLog();

	Encounter* CreateEncounter();

	bool Init(std::unique_ptr<EventPool> pool);
	void Shutdown();
	void Clear() override;

	void OnGameEvent(const GameEvent& evt);

	const std::vector<Encounter*>& GetEncounterList() const override
	{ 
		return m_Encounters; 
	}

	// Helpers for measuring gametime since a specific point of reference.
	Milliseconds GetTimeSince(const Frame frame, const Frame since) const;
	Milliseconds GetTimeSince(const Frame frame) const 
	{ 
		return GetTimeSince(frame, m_StartFrame); 
	}
	
	// Gets a 'HH:MM:SS' representation of an encounters duration.
	void GetDisplayString(const Encounter& encounter, DisplayString& buffer) const override;
	void GetDisplayString(const EncounterEvent& e, DisplayString& buffer) const override;

private:

	void OnGameStart();
	void OnGameEnd();
	void OnZoneEnter();
	void OnZoneExit();
	void OnCombatStart();
	void OnCombatEnd();
	void OnEntityCreated(const EntitySpawnedEvent& spawnEvent);
	void OnEntityDestroyed(const EntityDestroyedEvent& destroyEvent);
	void OnEntityDied(const DeathEvent& deathDevent);
	void OnDamageEvent(const DamageEvent& damageEvent);
	void OnUnitOccupancyChanged(const EntityOccupancyChangedEvent& occupancyEvent);
	void OnUnitPositionChanged(const EntityPositionChangedEvent& positionEvent);

	template <EncounterEventType T>
	EncounterEvent* CreateEncounterEvent()
	{
		return EncounterEvents::Create<T>(GetFrame(), m_EventPool.get());
	}

	bool AddEvent(EncounterEvent* evt);

	template <EncounterEventType T>
	EncounterEvent* AddEvent()
	{
		EncounterEvent* evt = nullptr;
		if (evt = CreateEncounterEvent<T>())
		{
			if (!AddEvent(evt))
			{
				delete evt;
				evt = nullptr;
			}
		}

		return evt;
	}

	template<EncounterEventType T, typename U>
	void HandleBasicEvent(const raid::GameEvent& src)
	{
		const U& evt = static_cast<const U&>(src);
		if (EncounterEvent* e = AddEvent<T>())
		{
			EncounterData::PackageData<U>(evt, *e);
		}
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

	Encounter* m_ActiveEncounter = nullptr;
	std::unique_ptr<EventPool> m_EventPool = nullptr;

	std::vector<Encounter*> m_Encounters;
	Frame m_StartFrame;
};


}