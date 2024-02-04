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

	m_OnCombatEvent = [this](CombatEvent* evt)
	{
		this->OnCombatEvent(evt);
	};

	system->CombatEventDlgt().Register(m_OnCombatEvent);
	return true;
}

void EncounterLog::Shutdown()
{
	ICombatSystem* system = Game::GetCombatSystem();
	if (!system)
		return;

	system->CombatEventDlgt().Unregister(m_OnCombatEvent);
}

void EncounterLog::OnCombatEvent(CombatEvent* evt)
{
	switch (evt->GetType())
	{
	case CombatEventType::CombatStart:
		break;
	case CombatEventType::CombatEnd:
		break;
	case CombatEventType::Damage:
		break;
	}
}

} // namespace raid