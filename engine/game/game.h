#pragma once

#include "engine/types.h"

namespace raid
{

// Forward declare Game classes
class IEntityManager;
class IDamageCalculator;
class ICombatSystem;
class IEncounterLog;
class IMap;
class World;

struct GameFrame
{
	World* World = nullptr;
	const Duration DeltaTime;
};

// Game Service Locators
class Game
{
public:

	// System to manage entities
	static void SetEntityManager(IEntityManager* locator);
	static IEntityManager* GetEntityManager();

	// System to calculate damage
	static void SetDamageCalculator(IDamageCalculator* calculator);
	static IDamageCalculator* GetDamageCalculator();

	// System to resolve combat
	static void SetCombatSystem(ICombatSystem* system);
	static ICombatSystem* GetCombatSystem();

	// Combat log (encounter log)
	static void SetEncounterLog(IEncounterLog* logger);
	static IEncounterLog* GetEncounterLog();

	// Map
	static void SetMap(IMap* map);
	static IMap* GetMap();

	// Game Frame
	static const GameFrame& GetGameFrame();

private:

	static IEntityManager* sm_EntityLocator;
	static IDamageCalculator* sm_DamageCalculator;
	static ICombatSystem* sm_CombatSystem;
	static IEncounterLog* sm_EncounterLog;
	static IMap* sm_Map;
	static GameFrame sm_GameFrame;
};



} // namespace raid