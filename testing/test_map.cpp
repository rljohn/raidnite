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