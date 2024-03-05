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
	const int64_t Frame;
	const TimeStep TimeStep;
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

class IGameSystem
{
public:

	virtual void Update(const GameFrame& frame) = 0;
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

	static void Init();
	static void Update(const GameFrame& frame);
	static void Shutdown();

	// Game Systems
	static void RegisterGameSystem(IGameSystem* system);
	static void UnregisterGameSystem(IGameSystem* system);

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

	// Game Events
	static GameEventDelegate& GameEventDlgt() { return sm_GameEventDlgt; }
	static void DispatchGameEvent(const GameEvent* evt);

private:

	static IEntityManager* sm_EntityLocator;
	static IDamageCalculator* sm_DamageCalculator;
	static ICombatSystem* sm_CombatSystem;
	static IEncounterLog* sm_EncounterLog;
	static Map* sm_Map;

	static GameEventDelegate sm_GameEventDlgt;
	static std::vector<IGameSystem*> sm_GameSystems;
};



} // namespace raid