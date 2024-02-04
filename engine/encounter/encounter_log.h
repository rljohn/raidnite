#pragma once

#include "engine/game/combat.h"

namespace raid
{

class EncounterLog
{
public:

	bool Init();
	void Shutdown();

private:

	void OnCombatEvent(CombatEvent* evt);

	ICombatSystem::CombatEventDelegate::Function m_OnCombatEvent;
};

}