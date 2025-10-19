#include "pch.h"
#include "test_game.h"

#include "engine/game/damage.h"
#include "engine/game/game.h"
#include "engine/game/world.h"
#include "engine/unit/spawner.h"
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


TEST_F(GameTest, SpawnUnit)
{
	UnitSpawner spawner;

	Entity* e = spawner.SpawnEntity(nullptr, Position(0,0));
	EXPECT_NE(e, nullptr);

	EXPECT_EQ(m_World.FindEntity(e->GetId()), e);

	m_World.UnRegisterEntity(e);
	EXPECT_EQ(m_World.FindEntity(e->GetId()), nullptr);

	delete e;
}

TEST_F(GameTest, TargetScan)
{
	WorldRAII world;
	FactionManagerRAII factions;

	constexpr const FactionId FriendlyFaction = 1;
	constexpr const FactionId EnemyFaction = 2;

	factions.Instance.SetRelationship(FriendlyFaction, EnemyFaction, FactionRelationship::Hostile);

	Unit source;
	Unit targetA, targetB, targetC;
	Unit friendly;

	std::vector<std::reference_wrapper<Unit>> units = { source, targetA, targetB, targetC, friendly };
	for (Unit& u : units)
	{
		world.Instance.RegisterEntity(&u);
		u.GetAttributes().SetupAttributes();
	}

	source.GetTransform().SetPosition(Position(0, 0));
	source.GetAttribute<AttributeType::Faction>()->SetValue(FriendlyFaction);

	// in rnageB
	targetA.GetTransform().SetPosition(Position(2, 0));
	targetA.GetAttribute<AttributeType::Faction>()->SetValue(EnemyFaction);

	// in range
	targetB.GetTransform().SetPosition(Position(2, 1));
	targetB.GetAttribute<AttributeType::Faction>()->SetValue(EnemyFaction);

	// out of range
	targetC.GetTransform().SetPosition(Position(2, 3));
	targetC.GetAttribute<AttributeType::Faction>()->SetValue(EnemyFaction);

	// friendly target - in range
	friendly.GetTransform().SetPosition(Position(1, 1));
	friendly.GetAttribute<AttributeType::Faction>()->SetValue(FriendlyFaction);

	AIComponent* ai = source.CreateAi<AIComponent>();
	ASSERT_NE(ai, nullptr);

	TargetScanParams params;
	params.Range = 2;
	params.Type = TargetFilter::Enemy;

	Entity* target = ai->ScanForTarget(params);
	EXPECT_EQ(target, &targetA);

	std::vector<Entity*> targets = ai->ScanForTargets(params);
	EXPECT_EQ(targets.size(), 2);

	// Friendly Scan
	params.Type = TargetFilter::Friendly;
	target = ai->ScanForTarget(params);
	EXPECT_EQ(target, &friendly);
}

TEST_F(GameTest, Aggro)
{
	WorldRAII world;
	FactionManagerRAII factions;

	constexpr const FactionId FriendlyFaction = 1;
	constexpr const FactionId EnemyFaction = 2;

	factions.Instance.SetRelationship(FriendlyFaction, EnemyFaction, FactionRelationship::Hostile);

	Unit player, enemy;

	std::vector<std::reference_wrapper<Unit>> units = { player, enemy };
	for (Unit& u : units)
	{
		world.Instance.RegisterEntity(&u);
		u.GetAttributes().SetupAttributes();
	}

	player.CreateAi<AIComponent>();
	player.GetTransform().SetPosition(Position(0, 0));
	player.GetAttribute<AttributeType::Faction>()->SetValue(FriendlyFaction);

	enemy.CreateAi<AIComponent>();
	enemy.GetTransform().SetPosition(Position(4, 0));
	enemy.GetAttribute<AttributeType::Faction>()->SetValue(EnemyFaction);
	enemy.GetAggro().SetBehaviour(AggroBehaviour::Aggro);

	GameFrame frame
	{
		0, Milliseconds(16), std::chrono::duration_cast<TimeStepSeconds>(Milliseconds(16))
	};

	// Tick for "160ms" as Aggro Component ticks every 100ms
	for (int i = 0; i < 10; i++)
	{
		world.Instance.Update(frame);
	}

	EXPECT_TRUE(enemy.GetAggro().Count() == 1);
	EXPECT_EQ(enemy.GetTargeting().GetTarget(), &player);
}

TEST_F(GameTest, BasicAttack)
{
	WorldRAII world;
	FactionManagerRAII factions;

	constexpr const FactionId FriendlyFaction = 1;
	constexpr const FactionId EnemyFaction = 2;

	factions.Instance.SetRelationship(FriendlyFaction, EnemyFaction, FactionRelationship::Hostile);

	Unit player, enemy;

	std::vector<std::reference_wrapper<Unit>> units = { player, enemy };
	for (Unit& u : units)
	{
		world.Instance.RegisterEntity(&u);
		u.GetAttributes().SetupAttributes();
	}

	player.CreateAi<AIComponent>();
	player.GetTransform().SetPosition(Position(0, 0));
	player.GetAttribute<AttributeType::Faction>()->SetValue(FriendlyFaction);
	player.GetAggro().SetBehaviour(AggroBehaviour::Aggro);

	enemy.CreateAi<AIComponent>();
	enemy.GetTransform().SetPosition(Position(4, 0));
	enemy.GetAttribute<AttributeType::Faction>()->SetValue(EnemyFaction);

	GameFrame frame
	{
		0, Milliseconds(16), std::chrono::duration_cast<TimeStepSeconds>(Milliseconds(16))
	};

	// Tick for "160ms" as Aggro Component ticks every 100ms
	for (int i = 0; i < 10; i++)
	{
		world.Instance.Update(frame);
	}
}