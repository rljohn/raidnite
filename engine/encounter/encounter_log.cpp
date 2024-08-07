#include "engine/pch.h"
#include "encounter_log.h"

#include "engine/encounter/encounter.h"
#include "engine/game/combat.h"
#include "engine/system/log/logging.h"
#include "engine/system/check.h"
#include "engine/engine.h"

namespace raid
{

IEncounterLog::IEncounterLog()
	: m_StartFrame(0)
{
}

EncounterLog::EncounterLog()
	: IEncounterLog()
	, m_ActiveEncounter(nullptr)
	, m_EventPool(nullptr)
	, m_StartFrame(0)
{
}

EncounterLog::~EncounterLog()
{
	Shutdown();
}

bool EncounterLog::Init(std::unique_ptr<EventPool> pool)
{
	m_OnGameEvent = [this](const GameEvent* evt)
	{
		this->OnGameEvent(evt);
	};

	Game::GameEventDlgt().Register(m_OnGameEvent);
	
	m_EventPool = std::move(pool);
	return true;
}

void EncounterLog::Shutdown()
{
	Game::GameEventDlgt().Unregister(m_OnGameEvent);
	m_OnGameEvent = nullptr;
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
		e->Shutdown(m_EventPool.get());

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
	case GameEventType::UnitSpawned:
		{
			const UnitSpawnedEvent* spawnEvent = static_cast<const UnitSpawnedEvent*>(evt);
			OnEntityCreated(spawnEvent);
		}
		break;
	case GameEventType::UnitDestroyed:
		{
			const UnitDestroyedEvent* destroyEvent = static_cast<const UnitDestroyedEvent*>(evt);
			OnEntityDestroyed(destroyEvent);
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

		EncounterEvent* start = CreateEncounterEvent<EncounterEventType::GameStart>();
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

void EncounterLog::OnEntityCreated(const UnitSpawnedEvent* spawnEvent)
{
	AddEvent<EncounterEventType::EntityCreated>();
}

void EncounterLog::OnEntityDestroyed(const UnitDestroyedEvent* destroyEvent)
{
	AddEvent<EncounterEventType::EntityDestroyed>();
}

void EncounterLog::OnEntityDied(const DeathEvent* deathDevent)
{

}

void EncounterLog::OnDamageEvent(const DamageEvent* damageEvent)
{

}

Milliseconds EncounterLog::GetTimeSince(const Frame frame, const Frame since) const
{
	if (Engine* engine = Game::GetEngine())
	{
		Frame duration = frame - since;
		return engine->FramesToMillis(duration);
	}

	return Milliseconds(0);
}

void EncounterLog::GetDisplayString(const Encounter& e, DisplayString& buffer) const
{
	Time::TimeDisplay startDisplay, endDisplay, durationDisplay;

	if (e.GetEndFrame() != 0)
	{
		Frame dt = e.GetEndFrame() - e.GetStartFrame();

		Milliseconds start = GetTimeSince(e.GetStartFrame());
		Milliseconds end = GetTimeSince(e.GetEndFrame());
		Milliseconds duration = GetTimeSince(e.GetEndFrame(), e.GetStartFrame());
		
		Time::GetHMS(start, startDisplay);
		Time::GetHMS(end, endDisplay);
		Time::GetHMS(duration, durationDisplay);

		sprintf_s(buffer, "%s - %s (%s)", startDisplay, endDisplay, durationDisplay);
	}
	else
	{
		mainAssert(e.GetStartFrame() != 0);

		Milliseconds start = GetTimeSince(e.GetStartFrame());
		Milliseconds duration = GetTimeSince(GetFrame(), e.GetStartFrame());

		Time::GetHMS(start, startDisplay);
		Time::GetHMS(duration, durationDisplay);

		sprintf_s(buffer, "%s (%s)", startDisplay, durationDisplay);
	}
}

void EncounterLog::GetDisplayString(const EncounterEvent& e, DisplayString& buffer) const
{
	Milliseconds ts = GetTimeSince(e.m_Frame);

	Time::TimeDisplay display = { 0 };
	Time::GetHMS(ts, display);

	strcpy_s(buffer, display);
}

} // namespace raid