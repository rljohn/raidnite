#include "engine/pch.h"
#include "encounter_log.h"

#include "engine/encounter/encounter.h"
#include "engine/game/combat.h"
#include "engine/system/check.h"

namespace raid
{

EncounterLog::EncounterLog()
	: m_ActiveEncounter(nullptr)
	, m_EventPool(nullptr)
{
}

bool EncounterLog::Init(EventPool* pool)
{
	ICombatSystem* system = Game::GetCombatSystem();
	if (!system)
		return false;

	m_OnGameEvent = [this](GameEvent* evt)
	{
		this->OnGameEvent(evt);
	};

	system->GameEventDlgt().Register(m_OnGameEvent);
	
	m_EventPool = pool;
	return true;
}

void EncounterLog::Shutdown()
{
	ICombatSystem* system = Game::GetCombatSystem();
	if (!system)
		return;

	system->GameEventDlgt().Unregister(m_OnGameEvent);
	m_OnGameEvent = nullptr;

	delete m_EventPool;
	m_EventPool = nullptr;
}

Encounter* EncounterLog::CreateEncounter()
{
	if (m_ActiveEncounter = new Encounter())
	{
		m_Encounters.push_back(m_ActiveEncounter);
	}
	
	return m_ActiveEncounter;
}

void EncounterLog::Clear()
{
	while (!m_Encounters.empty())
	{
		Encounter* e = m_Encounters.back();
		e->Shutdown(m_EventPool);

		m_Encounters.pop_back();
	}
}

void EncounterLog::OnGameEvent(GameEvent* evt)
{
	switch (evt->GetType())
	{
	case GameEventType::ZoneEnter:
		OnZoneEnter(); 
		break;
	case GameEventType::ZoneExit:
		OnZoneExit();
		break;
	case GameEventType::CombatStart:
		OnCombatStart();
		break;
	case GameEventType::CombatEnd:
		OnCombatEnd();
		break;
	case GameEventType::Damage:
		break;
	case GameEventType::Death:
		{
			DeathEvent* deathDevent = static_cast<DeathEvent*>(evt);
			OnEntityDied(deathDevent);
		}
		break;
	}
}

void EncounterLog::OnZoneEnter()
{
}

void EncounterLog::OnZoneExit()
{

}

void EncounterLog::OnCombatStart()
{
	stry
	{
		CreateEncounter();
		scheckall(m_ActiveEncounter);

		EncounterEvent* start = CreateEvent<EncounterEventType::EncounterStart>();
		scheckall(start);
		m_ActiveEncounter->AddEvent(start);
	}
	sfinally
	{
	}
}

void EncounterLog::OnCombatEnd()
{
	if (m_ActiveEncounter)
	{
		if (EncounterEvent* end = CreateEvent<EncounterEventType::EncounterEnd>())
		{
			m_ActiveEncounter->AddEvent(end);
		}
	}

	m_ActiveEncounter = nullptr;
}

void EncounterLog::OnEntityDied(DeathEvent* deathDevent)
{

}

void EncounterLog::OnDamageEvent(DamageEvent* damageEvent)
{

}

} // namespace raid