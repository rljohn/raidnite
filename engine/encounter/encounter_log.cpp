#include "engine/pch.h"
#include "encounter_log.h"

#include "engine/game/combat.h"

namespace raid
{

bool EncounterLog::Init()
{
	ICombatSystem* system = Game::GetCombatSystem();
	if (!system)
		return false;

	m_OnGameEvent = [this](GameEvent* evt)
	{
		this->OnGameEvent(evt);
	};

	system->GameEventDlgt().Register(m_OnGameEvent);
	return true;
}

void EncounterLog::Shutdown()
{
	ICombatSystem* system = Game::GetCombatSystem();
	if (!system)
		return;

	system->GameEventDlgt().Unregister(m_OnGameEvent);
	m_OnGameEvent = nullptr;
}

void EncounterLog::OnGameEvent(GameEvent* evt)
{
	switch (evt->GetType())
	{
	case GameEventType::CombatStart:
		break;
	case GameEventType::CombatEnd:
		break;
	case GameEventType::Damage:
		break;
	}
}

} // namespace raid