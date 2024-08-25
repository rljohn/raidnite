#include "pch.h"
#include "test.h"


#include "engine/engine.h"
#include "engine/game/game.h"
#include "engine/game/world.h"
#include "engine/map/map.h"
#include "engine/unit/spawner.h"

using namespace raid;

TEST(MovementTest, Basic)
{
	const std::chrono::nanoseconds frameTime(16666666);
	Engine engine;
	engine.Init(frameTime);

	World world;
	Map map;

	Game::SetMap(&map);
	Game::SetEntityManager(&world);
	Game::RegisterGameSystem(&world);

	map.BuildMap(10, 10);

	Position start(0, 0);

	UnitSpawner spawner;
	Unit* unit = dynamic_cast<Unit*>(spawner.SpawnEntity(&map, start));
	ASSERT_NE(unit, nullptr);

	// setup speed, 100 speed = 1 unit per second
	IAttribute* speed = unit->GetAttribute<AttributeType::Speed>();
	ASSERT_NE(speed, nullptr);
	speed->SetValue(100);

	// add AI to the component so it can process paths and movement
	AIComponent* ai = unit->CreateAi<AIComponent>();

	// set a target position of 5,0
	Position target(5, 0);
	ai->SetDesiredPosition(target);

	Tile* targetTile = map.GetTile(target);
	ASSERT_NE(targetTile, nullptr);
	ASSERT_EQ(targetTile->GetOccupant(), unit);

	MovementComponent* movement = unit->GetComponent<MovementComponent>();
	ASSERT_NE(movement, nullptr);
	const TilePath& path = movement->GetPath();

	// 0,0 - 1,0 - 2,0 - 3,0 - 4,0 - 5,0
	EXPECT_EQ(path.length(), 6);
	EXPECT_EQ(path[0]->GetPosition(), Position(0, 0));
	EXPECT_EQ(path[1]->GetPosition(), Position(1, 0));
	EXPECT_EQ(path[2]->GetPosition(), Position(2, 0));
	EXPECT_EQ(path[3]->GetPosition(), Position(3, 0));
	EXPECT_EQ(path[4]->GetPosition(), Position(4, 0));
	EXPECT_EQ(path[5]->GetPosition(), Position(5, 0));

	// tick once
	TimeStamp now = std::chrono::steady_clock::now();
	engine.Update(now, frameTime);
	now += frameTime;

	// TODO
	// EXPECT_TRUE(ai->IsMoving());

	// speed is set at 1 tile per second. We want to move 5 tiles = 5 seconds
	Position progress(start.GetX(), start.GetY());
	for (int tiles = 0; tiles < 5; tiles++)
	{
		const int iterations = Time::CountNanosInSeconds(frameTime, Seconds(1));
		for (int i = 0; i < iterations; i++)
		{
			engine.Update(now, frameTime);
			now += frameTime;
		}

		progress += Vector2D(1, 0);

		const Position& pos = unit->GetTransform().GetPosition();
		EXPECT_EQ(pos, progress);
	}

	EXPECT_EQ(unit->GetTransform().GetPosition(), target);
	spawner.DestroyEntity(&map, unit);

	Game::SetMap(nullptr);
	Game::SetEntityManager(nullptr);
	Game::UnregisterGameSystem(&world);
}

TEST(MovementTest, SkipFirstTile)
{
	const std::chrono::nanoseconds frameTime(16666666);
	
	Engine engine;
	engine.Init(frameTime);

	World world;
	Map map;

	Game::SetMap(&map);
	Game::SetEntityManager(&world);
	Game::RegisterGameSystem(&world);

	map.BuildMap(10, 10);

	Position start(0, 0);

	UnitSpawner spawner;
	Unit* unit = dynamic_cast<Unit*>(spawner.SpawnEntity(&map, start));
	ASSERT_NE(unit, nullptr);

	// setup speed, 100 speed = 1 unit per second
	IAttribute* speed = unit->GetAttribute<AttributeType::Speed>();
	ASSERT_NE(speed, nullptr);
	speed->SetValue(100);

	// add AI to the component so it can process paths and movement
	AIComponent* ai = unit->CreateAi<AIComponent>();

	TimeStamp now = std::chrono::steady_clock::now();

	// set a target position of 5,0
	Position target(5, 0);
	ai->SetDesiredPosition(target);

	// start moving to the new position - 400ms is enough to move nearly half a tile
	const int iterations = Time::CountNanosInMillis(frameTime, Milliseconds(400));
	for (int i = 0; i < iterations; i++)
	{
		engine.Update(now, frameTime);
		now += frameTime;
	}

	target = Position(3, 3);
	ai->SetDesiredPosition(target);

	MovementComponent* mover = unit->GetComponent<MovementComponent>();
	ASSERT_NE(mover, nullptr);

	const TilePath& path = mover->GetPath();
	EXPECT_EQ(path[0]->GetPosition(), Position(0, 0));
	EXPECT_EQ(path[1]->GetPosition(), Position(1, 1));
	EXPECT_EQ(mover->GetTilePathIndex(), 1);
	
	spawner.DestroyEntity(&map, unit);

	Game::SetMap(nullptr);
	Game::SetEntityManager(nullptr);
	Game::UnregisterGameSystem(&world);
}

TEST(MovementTest, AllowMovement)
{
	const std::chrono::nanoseconds frameTime(16666666);

	Engine engine;
	engine.Init(frameTime);

	World world;
	Map map;

	Game::SetMap(&map);
	Game::SetEntityManager(&world);
	Game::RegisterGameSystem(&world);

	map.BuildMap(10, 10);

	Position start(0, 0);

	UnitSpawner spawner;
	Unit* unit = dynamic_cast<Unit*>(spawner.SpawnEntity(&map, start));
	ASSERT_NE(unit, nullptr);

	// Disable movement at 1,0
	Tile* t = map.GetTile(1, 0);
	ASSERT_NE(unit, nullptr);
	t->SetMovementEnabled(false);

	// add AI to the component so it can process paths and movement
	AIComponent* ai = unit->CreateAi<AIComponent>();
	ASSERT_NE(ai, nullptr);
	ai->SetDesiredPosition(Position(2, 0));

	// The path should flow: 0,0 -> 0,1 -> 1,1 -> 2,1 -> 2,0
	MovementComponent* mover = unit->GetComponent<MovementComponent>();
	ASSERT_NE(mover, nullptr);

	const TilePath& path = mover->GetPath();
	ASSERT_EQ(path.length(), 5);
	EXPECT_EQ(path[0]->GetPosition(), Position(0, 0));
	EXPECT_EQ(path[1]->GetPosition(), Position(0, 1));
	EXPECT_EQ(path[2]->GetPosition(), Position(1, 1));
	EXPECT_EQ(path[3]->GetPosition(), Position(2, 1));
	EXPECT_EQ(path[4]->GetPosition(), Position(2, 0));

	spawner.DestroyEntity(&map, unit);

	Game::SetMap(nullptr);
	Game::SetEntityManager(nullptr);
	Game::UnregisterGameSystem(&world);
}

TEST(MovementTest, AllowOccupancy)
{
	const std::chrono::nanoseconds frameTime(16666666);

	Engine engine;
	engine.Init(frameTime);

	World world;
	
	Map map;

	Game::SetMap(&map);
	Game::SetEntityManager(&world);
	Game::RegisterGameSystem(&world);
	Game::RegisterGameEventListener(&world);

	map.BuildMap(10, 10);

	Position start(2, 2);

	UnitSpawner spawner;
	Unit* unit = dynamic_cast<Unit*>(spawner.SpawnEntity(&map, start));
	ASSERT_NE(unit, nullptr);

	// add AI to the component so it can process paths and movement
	AIComponent* ai = unit->CreateAi<AIComponent>();

	// Disable occupancy on this tile
	map.SetOccupancyAllowed(start, false);

	// We're moving to 1,1
	EXPECT_EQ(ai->GetDesiredPosition(), Position(1, 1));
	spawner.DestroyEntity(&map, unit);

	map.SetMovementAllowed(Position(1, 1), false);
	EXPECT_EQ(unit->GetTransform().GetPosition(), Position(0, 0));

	Game::SetMap(nullptr);
	Game::SetEntityManager(nullptr);
	Game::UnregisterGameSystem(&world);
}