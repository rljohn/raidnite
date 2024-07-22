#pragma once

#include "engine/system/math/vector2.h"
#include "engine/types.h"

#include <unordered_map>
#include <unordered_set>

namespace std
{
	/* implement hash function so we can put FIntPoint into an unordered_set */
	template <> struct hash<raid::Position>
	{
		std::size_t operator()(const raid::Position& id) const noexcept
		{
			// NOTE: better to use something like boost hash_combine
			return std::hash<int>()(id.GetX() ^ (id.GetY() << 16));
		}
	};
}

namespace raid
{
	class Tile;
	class Map;

	struct TilePath
	{
		void Reset();

		Tile* GetDestination();

		void operator=(const TilePath& other)
		{
			this->m_Tiles = other.m_Tiles;
		}

		const Tile* operator[](int index) const
		{
			return m_Tiles[index];
		}

		Tile* operator[](int index)
		{
			return m_Tiles[index];
		}

		size_t length() const { return m_Tiles.size(); }

		std::vector<Tile*> m_Tiles;
	};


	class Pathfinding
	{
	public:

		// Gets the tile distances between two tiles
		static Position TileDistanceBetweenTiles(const Position& first, const Position& second);

		// Returns the absolute tile distance between two positions.
		static double DistanceBetweenTiles(const Position& first, const Position& second);

		// Performs A* pathfinding from the starting point to the goal point.
		// Pair with ReconstructPath to build a path from the start to the goal.
		static void AStarSearch(Map* graph, const Position& start, const Position& goal,
			std::unordered_map<Position, Position>& came_from, std::unordered_map<Position, double>& cost_so_far);

		// From an A* pathfinding result, generate a path from start to goal.
		static void ReconstructPath(Map* graph, const Position& start, const Position& goal,
			std::unordered_map<Position, Position> came_from, TilePath& out_path);

		// Finds the closest available tile to a specific target. The 'start' point is used as a hint for
		// which nearby tiles to prefer.
		static bool FindClosestTile(Map* graph, const Position& start, const Position& goal, Position& out_result);

		// Checks if two tiles can be swapped (they are next door neighbours)
		static bool CanSwapTiles(const Position& first, const Position& second);
	};

} // namespace raid