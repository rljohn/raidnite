#include "engine/pch.h"
#include "game.h"

#include "engine/entity/entity.h"
#include "engine/game/damage.h"
#include "engine/game/combat.h"

namespace raid
{

std::vector<IGameSystem*> Game::sm_GameSystems;
IEntityManager* Game::sm_EntityLocator = nullptr;
IDamageCalculator* Game::sm_DamageCalculator = nullptr;
ICombatSystem* Game::sm_CombatSystem = nullptr;
IEncounterLog* Game::sm_EncounterLog = nullptr;
Map* Game::sm_Map = nullptr;

GameEventDelegate Game::sm_GameEventDlgt;


void Game::Init()
{

}

void Game::Update(const GameFrame& frame)
{
	for (IGameSystem* system : sm_GameSystems)
	{
		system->Update(frame);
	}
}

void Game::Shutdown()
{
	sm_GameSystems.clear();
}

void Game::RegisterGameSystem(IGameSystem* system)
{
	sm_GameSystems.push_back(system);
}

void Game::UnregisterGameSystem(IGameSystem* system)
{
	VectorRemove(sm_GameSystems, system);
}

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

void Game::SetMap(Map* map)
{
	sm_Map = map;
}

Map* Game::GetMap()
{
	return sm_Map;
}

void Game::DispatchGameEvent(const GameEvent* evt)
{
	sm_GameEventDlgt.Invoke(evt);
}

} // namespace raid