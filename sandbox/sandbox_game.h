#pragma once

// engine
#include "engine/game/game.h"
#include "engine/game/world.h"
#include "engine/game/combat.h"
#include "engine/encounter/encounter_log.h"

// widgets
#include "map_widgets.h"

namespace raid {
namespace sandbox {

class GameSandbox
{
public:

	void Init();
	void Update();
	void Shutdown();

private:

	raid::World m_World;
	raid::EncounterLog m_EncounterLog;
	raid::DamageCalculator m_DamageCalculator;
	raid::CombatSystem m_CombatSystem;

	// Widgets
	std::vector<Widget*> m_Widgets;
	MapWidget m_MapWidgets;
};

} // namespace sandbox
} // namespace raid