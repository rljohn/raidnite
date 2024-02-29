#pragma once

#include "engine/types.h"
#include "engine/system/delegate.h"

namespace raid
{

// Forward declare Game classes
class IEntityManager;
class IDamageCalculator;
class ICombatSystem;
class IEncounterLog;
class Map;
class World;

struct GameFrame
{
	World* World = nullptr;
	const Duration DeltaTime;
};

enum class GameEventType
{
	Invalid = 0,
	ZoneEnter,
	ZoneExit,
	CombatStart,
	CombatEnd,
	Damage,
	Death
};

struct GameEvent
{
	virtual GameEventType GetType() const = 0;
};


using GameEventDelegate = Delegate<const GameEvent*>;

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
	static void SetMap(Map* map);
	static Map* GetMap();

	// Game Frame
	static const GameFrame& GetGameFrame();

	static GameEventDelegate& GameEventDlgt() { return sm_GameEventDlgt; }
	static void DispatchGameEvent(const GameEvent* evt);

private:

	static IEntityManager* sm_EntityLocator;
	static IDamageCalculator* sm_DamageCalculator;
	static ICombatSystem* sm_CombatSystem;
	static IEncounterLog* sm_EncounterLog;
	static Map* sm_Map;

	static GameEventDelegate sm_GameEventDlgt;
	static GameFrame sm_GameFrame;
};



} // namespace raid