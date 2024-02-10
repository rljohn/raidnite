#include "pch.h"

#include "engine/encounter/encounter_log.h"
#include "engine/game/combat.h"

#include "test.h"

using namespace raid;

TEST(EncounterLogTest, InitShutdown)
{
	CombatSystemRAII combat;

	EncounterLog log;
	log.Init();
	EXPECT_EQ(combat.Instance.GameEventDlgt().GetCount(), 1);

	log.Shutdown();
	EXPECT_EQ(combat.Instance.GameEventDlgt().GetCount(), 0);
}

TEST(EncounterLogTest, StartEnd)
{
	CombatSystemRAII combat;

	EncounterLog log;
}