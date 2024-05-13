#pragma once

// engine
#include "engine/engine.h"
#include "engine/game/game.h"
#include "engine/game/game_instance.h"
#include "engine/game/world.h"
#include "engine/game/combat.h"
#include "engine/unit/group.h"
#include "engine/unit/spawner.h"
#include "engine/encounter/encounter_log.h"

// widgets
#include "engine_widgets.h"
#include "log_widgets.h"
#include "map_widgets.h"
#include "party_widgets.h"
#include "game_instance_widgets.h"

namespace raid {
namespace sandbox {

class GameSandbox
{
public:

	void Init();
	void Update();
	void Shutdown();

	raid::Engine& GetEngine() { return m_Engine; }
	raid::GameInstance& GetGameInstance() { return m_GameInstance; }
	raid::Group& GetParty() { return m_Party; }
	raid::UnitSpawner& GetUnitSpawner() { return m_UnitSpawner; }

private:

	raid::Engine m_Engine;

	// Subsystems
	raid::World m_World;
	raid::EncounterLog m_EncounterLog;
	raid::DamageCalculator m_DamageCalculator;
	raid::CombatSystem m_CombatSystem;
	raid::GameInstance m_GameInstance;

	// Game
	raid::Group m_Party;
	raid::UnitSpawner m_UnitSpawner;

	// Widgets
	std::vector<Widget*> m_Widgets;
	MapWidget m_MapWidgets;
	PartyWidget m_PartyWidgets;
	EngineWidget m_EngineWidgets;
	LogWidget m_LogWidgets;
	GameInstanceWidget m_GameInstanceWidgets;
};

} // namespace sandbox
} // namespace raid