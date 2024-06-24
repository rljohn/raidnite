#include "engine/pch.h"
#include "encounter_log.h"

#include "engine/encounter/encounter.h"
#include "engine/game/combat.h"
#include "engine/system/log/logging.h"
#include "engine/system/check.h"
#include "engine/engine.h"

namespace raid
{

EncounterLog::EncounterLog()
	: m_ActiveEncounter(nullptr)
	, m_EventPool(nullptr)
	, m_StartFrame(0)
{
}

bool EncounterLog::Init(EventPool* pool)
{
	m_OnGameEvent = [this](const GameEvent* evt)
	{
		this->OnGameEvent(evt);
	};

	Game::GameEventDlgt().Register(m_OnGameEvent);
	
	m_EventPool = pool;
	return true;
}

void EncounterLog::Shutdown()
{
	Game::GameEventDlgt().Unregister(m_OnGameEvent);
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

bool EncounterLog::AddEvent(EncounterEvent * evt)
{
	mainAssert(evt);
	mainAssert(m_ActiveEncounter);

	if (m_ActiveEncounter && evt)
	{
		m_ActiveEncounter->AddEvent(evt);
		return true;
	}
	else
	{
		return false;
	}
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

void EncounterLog::OnGameEvent(const GameEvent* evt)
{
	switch (evt->GetType())
	{
	case GameEventType::GameStart:
		OnGameStart();
		break;
	case GameEventType::GameEnd:
		OnGameEnd();
		break;
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
			const DeathEvent* deathDevent = static_cast<const DeathEvent*>(evt);
			OnEntityDied(deathDevent);
		}
		break;
	}
}

void EncounterLog::OnGameStart()
{
	stry
	{
		CreateEncounter();
		scheckall(m_ActiveEncounter);
		m_ActiveEncounter->Begin(GetFrame(), false);

		EncounterEvent* start = CreateEvent<EncounterEventType::GameStart>();
		scheckall(start);
		m_ActiveEncounter->AddEvent(start);

		m_StartFrame = GetFrame();
	}
	sfinally
	{
	}
}

void EncounterLog::OnGameEnd()
{
	mainAssert(m_ActiveEncounter);
	if (m_ActiveEncounter)
	{
		AddEvent<EncounterEventType::GameEnd>(); 
		m_ActiveEncounter->End(GetFrame());
		m_ActiveEncounter = nullptr;
	}
}

void EncounterLog::OnZoneEnter()
{
	AddEvent<EncounterEventType::ZoneEnter>();
}

void EncounterLog::OnZoneExit()
{
	AddEvent<EncounterEventType::ZoneExit>();
}

Frame EncounterLog::GetFrame() const
{
	if (Engine* engine = Game::GetEngine())
	{
		return engine->GetFrameCount();
	}

	return 0;
}

void EncounterLog::OnCombatStart()
{
	CreateEncounter();
	AddEvent<EncounterEventType::EncounterStart>();
}

void EncounterLog::OnCombatEnd()  
{
	AddEvent<EncounterEventType::EncounterEnd>();
	m_ActiveEncounter = nullptr;
}

void EncounterLog::OnEntityDied(const DeathEvent* deathDevent)
{

}

void EncounterLog::OnDamageEvent(const DamageEvent* damageEvent)
{

}

Milliseconds EncounterLog::GetTimeSince(const Frame frame) const
{
	if (Engine* engine = Game::GetEngine())
	{
		Frame duration = frame - m_StartFrame;
		return engine->FramesToMillis(duration);
	}

	return Milliseconds(0);
}

} // namespace raid