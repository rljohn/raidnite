#include "pch.h"

#include "engine/map/map.h"

#include "test.h"

using namespace raid;

TEST(MapTest, InitMap)
{
	Map map;
	map.BuildMap(10, 10);

	// out of range
	EXPECT_EQ(map.GetTile(-1, -1), nullptr);
	EXPECT_EQ(map.GetTile(-1, 0), nullptr);
	EXPECT_EQ(map.GetTile(0, -1), nullptr);
	EXPECT_EQ(map.GetTile(10, 0), nullptr);
	EXPECT_EQ(map.GetTile(0, 10), nullptr);
	EXPECT_EQ(map.GetTile(10, 10), nullptr);

	// in range
	EXPECT_NE(map.GetTile(0, 0), nullptr);
	EXPECT_NE(map.GetTile(0, 9), nullptr);
	EXPECT_NE(map.GetTile(9, 9), nullptr);
	EXPECT_NE(map.GetTile(9, 0), nullptr);

	// values should be filled
	EXPECT_EQ(map.GetTile(5, 5)->GetPosition(), Position(5,5));
}

TEST(MapTest, NearestAvailableTile)
{
	Map map;
	map.BuildMap(5, 5);

	Position pos(2, 2);

	// Place an entity on the tile at position 2,2
	Entity e;
	Tile* tile = map.GetTile(pos);
	tile->SetOccupant(&e);
	EXPECT_TRUE(tile->IsOccupied());

	/*	xxxxx
	*	xxxxx
	*	xxoxx
	*	xxxxx
	*	xxxxx
	*/

	// Order is: Right, Left, Down, Up, UpRight, DownLeft, UpLeft, DownRight

	Position offsets[] =
	{
		Position(1,0),		// 3,2
		Position(-1, 0),	// 1,2
		Position(0, -1),	// 2,1
		Position(0, 1),		// 2,3
		Position(1, 1),		// 3,3
		Position(-1, -1),	// 1,1
		Position(-1, 1),	// 1,3
		Position(1, -1)		// 3,1
	};

	for (const Position& offset : offsets)
	{
		Position result;
		bool success = map.GetNearestUnoccupiedTile(pos, result);
		EXPECT_TRUE(success);
		EXPECT_EQ(result, pos + offset);
		map.GetTile(result)->SetOccupant(&e);
	}
}

TEST(MapTest, NearestAvailableTileFrom)
{
	Map map;
	map.BuildMap(5, 5);

	Position pos(2, 2);

	// Place an entity on the tile at position 2,2
	Entity e;
	Tile* tile = map.GetTile(pos);
	tile->SetOccupant(&e);
	EXPECT_TRUE(tile->IsOccupied());

	/*	xxxxx
	*	xxxxx
	*	xxoxx
	*	xxxxx
	*	xxYxx
	*/

	// Default Order is: Right, Left, Down, Up, DownRight, UpLeft, DownLeft, UpRight
	// Unlike the previous test, we want to factor in the unit's current position
	// and ensure it chooses the available position that is closest to that.
	Position start(2, 4);

	Position offsets[] =
	{
		Position(0, 1),
		Position(1, 1),
		Position(-1, 1),
		Position(1, 0),
		Position(-1, 0),
		Position(0, -1),
		Position(-1, -1),
		Position(1, -1),
	};

	// Fill in each tile to prove the tile filling algorithms is working correctly.
	for (const Position& offset : offsets)
	{
		Position result;
		bool success = map.GetNearestUnoccupiedTile(pos, start, result);
		EXPECT_TRUE(success);

		Position expectation = pos + offset;
		EXPECT_EQ(result.GetX(), expectation.GetX());
		EXPECT_EQ(result.GetY(), expectation.GetY());
		map.GetTile(result)->SetOccupant(&e);
	}
}

