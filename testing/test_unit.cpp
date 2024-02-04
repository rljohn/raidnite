#include "pch.h"
#include "test.h"

#include "engine/game/game.h"
#include "engine/game/world.h"
#include "engine/unit/attribute.h"
#include "engine/unit/power.h"
#include "engine/unit/unit.h"
#include "engine/unit/effects/damage_effect.h"

using namespace raid;

TEST(UnitTest, PowerBasics)
{
	Power<PowerType::Health> p(0, 100);
	EXPECT_EQ(p.GetCurrent(), 100);

	// Check clamping at max
	p.SetBaseValues(p.GetMin(), 50);
	EXPECT_EQ(p.GetCurrent(), 100);
	p.Recalculate();
	EXPECT_EQ(p.GetCurrent(), 50);

	// Empty the power to test min clamping
	p.SetEmpty();
	EXPECT_EQ(p.GetCurrent(), 0);
	EXPECT_TRUE(p.IsEmpty());

	// Check clamping at min
	p.SetBaseValues(25, p.GetMax());
	EXPECT_EQ(p.GetCurrent(), 0);
	p.Recalculate();
	EXPECT_EQ(p.GetCurrent(), 25);

	// Check refill
	p.ReFill();
	EXPECT_EQ(p.GetCurrent(), 50);
}

TEST(UnitTest, PowerPercent)
{
	Health p(0, 200);
	EXPECT_FALSE(p.NeedsRecalculate());

	// 50% HP check
	p.SetCurrent(100);
	EXPECT_FLOAT_EQ(p.GetPercent(), 50);

	// 10% hp check
	p.SetBaseValues(p.GetMin(), 1000);
	p.Recalculate();
	EXPECT_FLOAT_EQ(p.GetPercent(), 10);

	// 0% hp check
	p.SetEmpty();
	EXPECT_FLOAT_EQ(p.GetPercent(), 0);

	// 100%
	p.ReFill();
	EXPECT_FLOAT_EQ(p.GetPercent(), 100);
}

TEST(UnitTest, AttributeBasics)
{
	Stamina stam(100);
	EXPECT_FALSE(stam.NeedsRecalculate());
}

TEST(BuffTest, AttributeBuffs)
{
	Unit u;
	u.AddAttribute<AttributeType::Stamina>(100);

	IAttribute* stam = u.GetAttribute<AttributeType::Stamina>();
	ASSERT_NE(stam, nullptr);
	EXPECT_EQ(stam->GetValue(), 100);

	// Attribute buff that reduces Stamina by 10.
	BE_StaminaAddSubtract mod(-10);

	// construct the buff
	Buff buff;
	buff.AddEffect(&mod);

	// add buff - needs recalculate
	u.AddBuff(&buff);
	EXPECT_EQ(stam->GetValue(), 100);
	EXPECT_TRUE(stam->NeedsRecalculate());

	// recalculate
	stam->Recalculate();
	EXPECT_EQ(stam->GetValue(), 90);
}

TEST(BuffTest, PowerBuffs)
{
	// create a basic unit with only health
	Unit u;

	// initialze unit with 100 HP
	u.AddPower<PowerType::Health>(100);

	// Verify max HP is full
	IPower* hp = u.GetPower<PowerType::Health>();
	ASSERT_NE(hp, nullptr);
	EXPECT_EQ(hp->GetMax(), 100);

	// create a buff effect that halfs max health
	BE_HealthMultiplyDivideMax effect(0.5);

	// construct the buff
	Buff buff;
	buff.AddEffect(&effect);

	// add buff - needs recalculate
	u.AddBuff(&buff);
	EXPECT_TRUE(hp->NeedsRecalculate());

	// recalculate HP
	hp->Recalculate();
	EXPECT_EQ(hp->GetMax(), 50);
	EXPECT_EQ(hp->GetCurrent(), 50);

	// remove buff - needs recalculate
	u.RemoveBuff(&buff);
	EXPECT_TRUE(hp->NeedsRecalculate());
	hp->Recalculate();

	// Verify max HP is restored, but current is still 50
	EXPECT_EQ(hp->GetCurrent(), 50);
	EXPECT_EQ(hp->GetMax(), 100);
}

TEST(BuffTest, SortBuffs)
{
	Unit u;

	// initialze unit with 1000 HP
	u.AddPower<PowerType::Health>(1000);

	// Create two multipliers, two dividers
	BE_HealthMultiplyDivideMax mod1(0.5);
	BE_HealthMultiplyDivideMax mod2(0.5);
	BE_HealthAddSubtractMax mod3(-60);
	BE_HealthAddSubtractMax mod4(-30);

	// Add effects, alternating multiplication and addition
	Buff buff;
	buff.AddEffect(&mod3);
	buff.AddEffect(&mod1);
	buff.AddEffect(&mod2);
	buff.AddEffect(&mod4);

	// add buff - effects are sorted into BEDMAS
	u.AddBuff(&buff);

	// recalculate hit points
	IPower* hp = u.GetPower<PowerType::Health>();
	EXPECT_TRUE(hp->NeedsRecalculate());
	hp->Recalculate();

	// multiplication is first, then addition
	// HP should flow from: 1000 -> 500 -> 250 -> 190 -> 160
	EXPECT_EQ(hp->GetCurrent(), 160);
	
	// verify restore back to 1K HP
	u.RemoveBuff(&buff);
	hp->Recalculate();
	EXPECT_EQ(hp->GetMax(), 1000);
}

TEST(BuffTest, RestoreCurrentHp)
{
	Unit u;
	u.AddPower<PowerType::Health>(1000);

	// create a buff effect that halfs max health
	BE_HealthMultiplyDivideMax effect(0.5);
	BE_PowerRestoreOnRemoval<PowerType::Health> restore;

	Buff buff;
	buff.AddEffect(&effect);
	buff.AddEffect(&restore);
	u.AddBuff(&buff);

	// apply buff
	IPower* hp = u.GetPower<PowerType::Health>();
	hp->Recalculate();
	EXPECT_EQ(hp->GetMax(), 500);

	// remove buff - recalculation not needed
	u.RemoveBuff(&buff);
	EXPECT_FALSE(hp->NeedsRecalculate());
	EXPECT_EQ(hp->GetMax(), 1000);
}
