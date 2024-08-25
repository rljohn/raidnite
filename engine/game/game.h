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
class Engine;
class GameInstance;

struct GameFrame
{
	const int64_t Frame;
	const TimeStep TimeStep;
	const TimeStepSeconds TimeStepSecs;
};

enum class GameEventType
{
	Invalid = 0,
	GameStart,
	GameEnd,
	ZoneEnter,
	ZoneExit,
	CombatStart,
	CombatEnd,
	TilePropertiesChanged,
	Damage,
	Death,
	EntitySpawned,
	EntityDestroyed,
	EntityOccupancyChanged,
	EntityPositionChanged
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

class IGameEventListener
{
public:

	virtual void OnGameEvent(const GameEvent& evt) = 0;
};

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

	// Main engine
	static void SetEngine(Engine* engine);
	static Engine* GetEngine();

	// System to manage entities
	static void SetEntityManager(IEntityManager* locator);
	static IEntityManager* GetEntityManager();

	// System to calculate damages
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
	static void RegisterGameEventListener(IGameEventListener* listener);
	static void UnregisterGameEventListener(IGameEventListener* listener);
	static void DispatchGameEvent(const GameEvent& evt);

private:

	static Engine* sm_Engine;
	static IEntityManager* sm_EntityLocator;
	static IDamageCalculator* sm_DamageCalculator;
	static ICombatSystem* sm_CombatSystem;
	static IEncounterLog* sm_EncounterLog;
	static Map* sm_Map;

	static std::vector<IGameEventListener*> sm_GameEventListeners;
	static std::vector<IGameSystem*> sm_GameSystems;
};



} // namespace raid