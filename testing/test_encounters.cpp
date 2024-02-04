#include "pch.h"

#include "engine/encounter/encounter_log.h"
#include "engine/game/combat.h"

#include "test.h"

using namespace raid;

TEST(EncounterLog, InitShutdown)
{
	CombatSystem combat;
	Game::SetCombatSystem(&combat);

	EncounterLog log;
	log.Init();
	EXPECT_EQ(combat.CombatEventDlgt().GetCount(), 1);

	log.Shutdown();
	EXPECT_EQ(combat.CombatEventDlgt().GetCount(), 0);

	Game::SetCombatSystem(nullptr);
}