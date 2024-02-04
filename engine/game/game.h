#pragma once

#include "engine/types.h"

namespace raid
{

// Forward declare Game classes
class IEntityLocator;
class IDamageCalculator;
class ICombatSystem;
class IEncounterLog;

class World;

// Game Service Locators
class Game
{
public:

	// System to find entities
	static void SetEntityLocator(IEntityLocator* locator);
	static IEntityLocator* GetEntityLocator();

	// System to calculate damage
	static void SetDamageCalculator(IDamageCalculator* calculator);
	static IDamageCalculator* GetDamageCalculator();

	// System to resolve combat
	static void SetCombatSystem(ICombatSystem* system);
	static ICombatSystem* GetCombatSystem();

	// Combat log (encounter log)
	static void SetEncounterLog(IEncounterLog* logger);
	static IEncounterLog* GetEncounterLog();

private:

	static IEntityLocator* sm_EntityLocator;
	static IDamageCalculator* sm_DamageCalculator;
	static ICombatSystem* sm_CombatSystem;
	static IEncounterLog* sm_EncounterLog;
};

struct GameFrame
{
	World* World = nullptr;
	const Duration DeltaTime;
};

} // namespace raid