#include "pch.h"

#include "engine/encounter/encounter_log.h"
#include "engine/game/combat.h"
#include "engine/unit/unit.h"
#include "engine/game/world.h"

#include "test.h"

using namespace raid;

TEST(AggroTest, AddRemove)
{
	CombatSystemRAII combat;
	WorldRAII world;

	Unit a, b, c;

	world.Instance.RegisterEntity(&a);
	world.Instance.RegisterEntity(&b);
	world.Instance.RegisterEntity(&c);

	AggroComponent* aggroA = a.GetComponent<AggroComponent>();
	ASSERT_NE(aggroA, nullptr);

	// add a unit to the aggro table, confirm its addition
	aggroA->AddUnit(&b);
	EXPECT_EQ(aggroA->Count(), 1);
	EXPECT_FALSE(aggroA->Empty());

	// a is now in combat
	EXPECT_TRUE(a.IsInCombat());

	// kill the target entity
	combat.Instance.KillEntity(&b);

	// confirm the aggro table is gone
	EXPECT_EQ(aggroA->Count(), 0);
	EXPECT_TRUE(aggroA->Empty());

	// a is no longer in combat
	EXPECT_FALSE(a.IsInCombat());
}

