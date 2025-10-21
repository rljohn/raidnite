#include "pch.h"
#include "test.h"

#include "engine/engine.h"
#include "engine/game/ability.h"

TEST(AbilityTest, AbilityComponent)
{
	using namespace raid;

	Entity e;
	AbilityComponent& abilityComp = e.AddComponent<AbilityComponent>();

	const AbilityId TestAbilityId = 1337;

	AbilityDefinition abilityDef
	{
		.Id = TestAbilityId,
		.CostType = PowerType::Mana,
		.CostValue = 15,
		.Range = 3,
		.CastWhileMoving = true,
		.Damage = 100
	};

	abilityComp.AddAbility(abilityDef);
	
	Ability* a = abilityComp.GetAbility(TestAbilityId);
	ASSERT_NE(a, nullptr);
	EXPECT_EQ(a->GetId(), TestAbilityId);
	EXPECT_EQ(a->GetCostType(), PowerType::Mana);
	EXPECT_EQ(a->GetCostValue(), 15);
	EXPECT_EQ(a->GetRange(), 3);
	EXPECT_EQ(a->CanCastWhileMoving(), true);
	EXPECT_EQ(a->GetDamage(), 100);
}

TEST(AbilityTest, Cooldown)
{
	using namespace raid;

	Entity e;
	AbilityComponent& abilityComp = e.AddComponent<AbilityComponent>();

	const AbilityId TestAbilityId = 1337;

	AbilityDefinition abilityDef
	{
		.Id = TestAbilityId,
		.CostType = PowerType::Mana,
		.CostValue = 15,
		.Range = 3,
		.CastWhileMoving = true,
		.Targeting = AbilityTargetingFlags::EnemyTarget,
		.Damage = 100,
		.CooldownSeconds = 3.0,
		.UseGcd = true
	};

	abilityComp.AddAbility(abilityDef);

	Ability* a = abilityComp.GetAbility(TestAbilityId);
	ASSERT_NE(a, nullptr);

	abilityComp.SetCurrentAbility(a);

	const std::chrono::nanoseconds frameTime(16666666);
	EngineRAII engine;
	engine.Instance.Init(frameTime);
	
	EXPECT_TRUE(a->IsAvailable());
	
	TimeStamp now = std::chrono::steady_clock::now();
	engine.Instance.Update(now, frameTime);
	EXPECT_EQ(engine.Instance.GetFrameCount(), 1);

	//////////////////////////
	// Cast spell with 0% CDR
	//////////////////////////

	abilityComp.OnCast(engine.Instance.GetFrameCount(), 0.0);
	EXPECT_FALSE(a->IsAvailable());
	EXPECT_TRUE(a->IsOnCooldown());
	EXPECT_TRUE(abilityComp.IsOnGlobalCooldown());

	// Update engine until almost on cooldown (i.e. 1 tick away)
	Duration cd = Time::ToNanoSeconds(a->GetBaseCooldown());
	int ticks = static_cast<int>(std::floor(cd / frameTime)) - 1;
	for (int i = 0; i < ticks; i++)
	{
		engine.Instance.Update(now, frameTime);
	}
	EXPECT_FALSE(a->IsAvailable());
	EXPECT_TRUE(a->IsOnCooldown());
	EXPECT_FALSE(abilityComp.IsOnGlobalCooldown());

	// Update engine one more time - ability should be ready now
	engine.Instance.Update(now, frameTime);
	EXPECT_TRUE(a->IsAvailable());
	EXPECT_FALSE(a->IsOnCooldown());

	//////////////////////////
	// Cast spell with 20% CDR
	//////////////////////////

	double cdr = 0.2;
	abilityComp.OnCast(engine.Instance.GetFrameCount(), cdr);
	EXPECT_FALSE(a->IsAvailable());
	EXPECT_TRUE(a->IsOnCooldown());

	// Update engine until almost on cooldown (i.e. 1 tick away)
	cd = Time::ToNanoSeconds(a->GetBaseCooldown());
	ticks = static_cast<int>(((std::floor(cd / frameTime)) - 1) * (1 - cdr));
	for (int i = 0; i < ticks; i++)
	{
		engine.Instance.Update(now, frameTime);
	}
	EXPECT_FALSE(a->IsAvailable());
	EXPECT_TRUE(a->IsOnCooldown());

	// Update engine one more time - ability should be ready now
	engine.Instance.Update(now, frameTime);
	EXPECT_TRUE(a->IsAvailable());
	EXPECT_FALSE(a->IsOnCooldown());
}