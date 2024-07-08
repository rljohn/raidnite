#include "engine/pch.h"
#include "map.h"

#include "engine/system/log/logging.h"

namespace raid
{

void Map::BuildMap(const int width, const int height)
{
	m_Tiles.resize(width, std::vector<Tile>(height));

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			m_Tiles[i][j].SetPosition(Position(i, j));
		}
	}

	m_Width = width;
	m_Height = height;
}

void Map::Shutdown()
{
	m_Width = 0;
	m_Height = 0;
	m_Tiles.clear();
}

int Map::GetWidth() const
{
	return m_Width;
}

int Map::GetHeight() const
{
	return m_Height;
}

bool Map::IsPositionValid(const int x, const int y) const
{
	if (x < 0 || x >= GetWidth())
	{
		return false;
	}

	if (y < 0 || y >= GetHeight())
	{
		return false;
	}

	return true;
}

Tile* Map::GetTile(const int x, const int y)
{
	if (!IsPositionValid(x, y))
	{
		mapError("Invalid tile: {},{}", x, y);
		return nullptr;
	}

	return &m_Tiles[x][y];
}

Tile* Map::GetTile(const Position& position)
{
	return GetTile(position.GetX(), position.GetY());
}

bool Map::HasTile(const Position& position) const
{
	if (!IsPositionValid(position.GetX(), position.GetY()))
	{
		return false;
	}

	// TODO
	//	Check if this grid actually contains valid tile data
	return true;
}

bool Map::IsOccupied(const Position& position)
{
	Tile* tile = GetTile(position);
	return tile ? tile->IsOccupied() : false;
}

bool Map::IsAvailable(const Position& position)
{
	Tile* tile = GetTile(position);
	return tile && !tile->IsOccupied();
}

bool Map::BuildPath(Tile* start, Tile* end, TilePath& out_path)
{
	out_path.m_Tiles.clear();
	if (start == nullptr || end == nullptr)
	{
		out_path.Reset();
		return false;
	}

	const Position& startPos = start->GetPosition();
	const Position& endPos = end->GetPosition();

	// That tile isn't available, find another one
	if (!IsAvailable(endPos))
	{
		out_path.Reset();
		return false;
	}

	// Build a path
	std::unordered_map<Position, Position> came_from;
	std::unordered_map<Position, double> cost_so_far;
	Pathfinding::AStarSearch(this, startPos, endPos, came_from, cost_so_far);
	Pathfinding::ReconstructPath(this, startPos, endPos, came_from, out_path);
	return out_path.m_Tiles.size() > 0;
}

std::vector<Position> Map::GetNeighbors(const Position& pos) const
{
	auto IsPassable = [this](const Position& p)
	{
		return HasTile(p);
	};

	static std::array<Position, 4> DIRS =
	{
		/* East, West, North, South */
		Position(1, 0),
		Position(-1, 0),
		Position(0, -1),
		Position(0, 1),
	};

	std::vector<Position> results;
	results.reserve(8);

	for (const Position& dir : DIRS)
	{
		Position next{ pos.GetX() + dir.GetX(), pos.GetY() + dir.GetY()};
		if (IsPositionValid(next.GetX(), next.GetY()) && IsPassable(next))
		{
			results.push_back(next);
		}
	}

	static bool AllowDiagonalMotion = true;
	if (AllowDiagonalMotion)
	{
		struct DiagonalDirection
		{
			DiagonalDirection(Position p, Position r1, Position r2)
				: pos(p), req1(r1), req2(r2) {}

			Position pos;
			Position req1;
			Position req2;
		};

		static std::array<DiagonalDirection, 4> DIAGS =
		{
			/* Diagnonals */
			DiagonalDirection(Position(1, 1), Position(1, 0), Position(0, 1)),
			DiagonalDirection(Position(-1, -1), Position(-1, 0), Position(0, -1)),
			DiagonalDirection(Position(-1, 1), Position(-1, 0), Position(0, 1)),
			DiagonalDirection(Position(1, -1), Position(1, 0), Position(0, -1)),
		};

		for (const DiagonalDirection& dir : DIAGS)
		{
			Position next{ pos.GetX() + dir.pos.GetX(), pos.GetY() + dir.pos.GetY() };
			Position req1{ pos.GetX() + dir.req1.GetX(), pos.GetY() + dir.req1.GetY() };
			Position req2{ pos.GetX() + dir.req2.GetX(), pos.GetY() + dir.req2.GetY() };

			if (IsPositionValid(next.GetX(), next.GetY()) && IsPassable(next) && IsPassable(req1) && IsPassable(req2))
			{
				results.push_back(next);
			}
		}
	}

	return results;
}

double Map::GetCost(const Position& from, const Position& to)
{
	// TODO
	// Add hazard modifiers

	if (from.GetX() == to.GetX() || from.GetY() == to.GetY())
	{
		// Horiziontal
		return 1;
	}
	else
	{
		// Double
		return 1.001;
	}
}

bool Map::GetNearestUnoccupiedTile(const Position& target, Position& out_result)
{
	return GetNearestUnoccupiedTile(target, target, out_result);
}

bool Map::GetNearestUnoccupiedTile(const Position& target, const Position& from, Position& out_result)
{
	Position tmp;
	if (Pathfinding::FindClosestTile(this, from, target, tmp))
	{
		out_result = tmp;
		return true;
	}
	else
	{
		return false;
	}
}

} // namespace raid

