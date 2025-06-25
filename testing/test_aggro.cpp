#include "pch.h"

#include "engine/encounter/encounter_log.h"
#include "engine/game/combat.h"
#include "engine/game/faction.h"
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

TEST(AggroTest, AggroOnDamage)
{
	DamageCalculatorRAII calculator;
	CombatSystemRAII combat;
	FactionManagerRAII factions;

	WorldRAII world;
	GameEventListenerRAII listener(&world.Instance);

	Unit a, b, c;

	// setup some good guys and bad guys
	FactionId testGoodGuys = 1;
	FactionId testBadGuys = 2;
	factions.Instance.SetRelationship(testGoodGuys, testBadGuys, FactionRelationship::Hostile);

	// A is a good guy, B and C are bad guys
	a.GetAttributes().SetupAttributes();
	a.CreateAi<AIComponent>();
	if (IAttribute* fa = a.GetAttribute<AttributeType::Faction>())
	{
		fa->SetValue(testGoodGuys);
	}

	b.GetAttributes().SetupAttributes();
	b.CreateAi<AIComponent>();
	if (IAttribute* fb = b.GetAttribute<AttributeType::Faction>())
	{
		fb->SetValue(testBadGuys);
	}

	c.GetAttributes().SetupAttributes();
	c.CreateAi<AIComponent>();
	if (IAttribute* fc = c.GetAttribute<AttributeType::Faction>())
	{
		fc->SetValue(testBadGuys);
	}

	// Register in the world
	world.Instance.RegisterEntity(&a);
	world.Instance.RegisterEntity(&b);
	world.Instance.RegisterEntity(&c);

	AggroComponent* aggroA = a.GetComponent<AggroComponent>();
	ASSERT_NE(aggroA, nullptr);

	const AbilityId spellID = 1337;

	// Build a damage event from A to B
	CombatSystem::DamageParams params;
	params.Source = a.GetId();
	params.Target = b.GetId();
	params.Spell = spellID;
	params.Type = DamageType::Physical;
	params.SourceLevel = 10;
	params.Value = 100;
	combat.Instance.BuildDamageEvent(params);

	// damage should have put both players in combat
	EXPECT_TRUE(a.IsInCombat());
	EXPECT_TRUE(b.IsInCombat());

	// A and B should be targeting each other
	EXPECT_EQ(a.GetTargeting().GetTarget(), &b);
	EXPECT_EQ(b.GetTargeting().GetTarget(), &a);

	// damage from player A to player C should put player C in combat but not change player A's target
	params.Target = c.GetId();
	combat.Instance.BuildDamageEvent(params);
	EXPECT_TRUE(c.IsInCombat());
	EXPECT_EQ(a.GetTargeting().GetTarget(), &b);
	EXPECT_EQ(c.GetTargeting().GetTarget(), &a);

	// reset aggro
	a.GetAggro().ResetAggro();
	b.GetAggro().ResetAggro();
	c.GetAggro().ResetAggro();

	// out of combat
	EXPECT_FALSE(a.IsInCombat());
	EXPECT_FALSE(b.IsInCombat());
	EXPECT_FALSE(c.IsInCombat());
}