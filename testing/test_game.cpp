#include "pch.h"
#include "test_game.h"

#include "engine/game/damage.h"
#include "engine/game/world.h"
#include "engine/unit/unit.h"
#include "engine/unit/effects/damage_effect.h"

using namespace raid;

// SetUp and TearDown

void GameTest::SetUp()
{
	Game::SetEntityManager(&m_World);
	Game::SetDamageCalculator(&m_Calculator);
	Game::SetCombatSystem(&m_CombatSystem);
}

void GameTest::TearDown()
{
	Game::SetEntityManager(nullptr);
	Game::SetDamageCalculator(nullptr);
	Game::SetCombatSystem(nullptr);
}

TEST_F(GameTest, WorldBasics)
{
	ASSERT_NE(Game::GetDamageCalculator(), nullptr);
	ASSERT_NE(Game::GetEntityManager(), nullptr);
	ASSERT_NE(Game::GetCombatSystem(), nullptr);
}

TEST_F(GameTest, DamageEffects)
{
	// initialze unit with 1000 HP
	Unit target;
	target.AddPower<PowerType::Health>(1000);
	m_World.RegisterEntity(&target);

	Unit source;
	m_World.RegisterEntity(&source);

	// create a buff that does 500 damage on removal
	BE_Damage damage;
	damage.Init(source.GetId(), 0, DamageType::Fire, 500);

	Buff buff;
	buff.AddEffect(&damage);
	target.AddBuff(&buff);

	// Expire a buff to ensure its effect goes off
	target.GetComponent<BuffsComponent>()->ExpireBuff(&buff);

	// validate the damage applied
	IPower* hp = target.GetPower<PowerType::Health>();
	ASSERT_NE(hp, nullptr);
	EXPECT_EQ(hp->GetCurrent(), 500);
}