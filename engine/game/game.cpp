#include "engine/pch.h"
#include "game.h"

#include "engine/entity/entity.h"
#include "engine/game/damage.h"
#include "engine/game/combat.h"
#include "engine/engine.h"

namespace raid
{

std::vector<IGameSystem*> Game::sm_GameSystems;
std::vector<IGameEventListener*> Game::sm_GameEventListeners;
IEntityManager* Game::sm_EntityLocator = nullptr;
IDamageCalculator* Game::sm_DamageCalculator = nullptr;
ICombatSystem* Game::sm_CombatSystem = nullptr;
IEncounterLog* Game::sm_EncounterLog = nullptr;
IFactionManager* Game::sm_FactionManager = nullptr;
Map* Game::sm_Map = nullptr;
Engine* Game::sm_Engine = nullptr;

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

void Game::SetEngine(Engine* engine)
{
	sm_Engine = engine;
}

Engine* Game::GetEngine()
{
	return sm_Engine;
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

void Game::SetFactionManager(IFactionManager* mgr)
{
	sm_FactionManager = mgr;
}

IFactionManager* Game::GetFactionManager()
{
	return sm_FactionManager;
}

void Game::SetMap(Map* map)
{
	sm_Map = map;
}

Map* Game::GetMap()
{
	return sm_Map;
}

void Game::RegisterGameEventListener(IGameEventListener* listener)
{
	sm_GameEventListeners.push_back(listener);
}

void Game::UnregisterGameEventListener(IGameEventListener* listener)
{
	VectorRemove(sm_GameEventListeners, listener);
}

void Game::DispatchGameEvent(const GameEvent& evt)
{
	for (IGameEventListener* listener : sm_GameEventListeners)
	{
		listener->OnGameEvent(evt);
	}
}

} // namespace raid