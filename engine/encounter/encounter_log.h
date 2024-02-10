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

	void OnGameEvent(GameEvent* evt);

	ICombatSystem::GameEventDelegate::Function m_OnGameEvent;
};

}