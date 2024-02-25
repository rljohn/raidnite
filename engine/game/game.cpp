#include "engine/pch.h"
#include "game.h"

#include "engine/entity/entity.h"
#include "engine/game/damage.h"
#include "engine/game/combat.h"

namespace raid
{

IEntityManager* Game::sm_EntityLocator = nullptr;
IDamageCalculator* Game::sm_DamageCalculator = nullptr;
ICombatSystem* Game::sm_CombatSystem = nullptr;
IEncounterLog* Game::sm_EncounterLog = nullptr;
IMap* Game::sm_Map = nullptr;
GameFrame Game::sm_GameFrame;

void Game::SetEntityManager(IEntityManager* locator)
{
	sm_EntityLocator = locator;
}

IEntityManager* Game::GetEntityManager()
{
	return sm_EntityLocator;
}

void Game::SetDamageCalculator(IDamageCalculator* calculator)
{
	sm_DamageCalculator = calculator;
}

IDamageCalculator* Game::GetDamageCalculator()
{
	return sm_DamageCalculator;
}

void Game::SetCombatSystem(ICombatSystem* system)
{
	sm_CombatSystem = system;
}

ICombatSystem* Game::GetCombatSystem()
{
	return sm_CombatSystem;
}

void Game::SetEncounterLog(IEncounterLog* logger)
{
	sm_EncounterLog = logger;
}

IEncounterLog* Game::GetEncounterLog()
{
	return sm_EncounterLog;
}

void Game::SetMap(IMap* map)
{
	sm_Map = map;
}

IMap* Game::GetMap()
{
	return sm_Map;
}

const GameFrame& Game::GetGameFrame()
{
	return sm_GameFrame;
}

} // namespace raid