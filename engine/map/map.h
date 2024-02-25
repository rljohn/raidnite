#pragma once

#include "engine/types.h"
#include "engine/map/tile.h"
#include "engine/map/pathfinding.h"

namespace raid
{

class Map
{
public:

	void BuildMap(const int width, const int height);

	int GetWidth() const;
	int GetHeight() const;

	bool IsPositionValid(const int x, const int y) const;
	Tile* GetTile(const int x, const int y);
	Tile* GetTile(const Position& position);
	bool HasTile(const Position& position) const;

	void SetMapScale(const Vector2D& scale) { m_MapScale = scale; }
	const Vector2D& GetMapScale() const { return m_MapScale; }

	bool IsOccupied(const Position& position);
	bool IsAvailable(const Position& position);

	// Pathfinding
	bool BuildPath(Tile* start, Tile* end, TilePath& out_path);
	std::vector<Position> GetNeighbors(const Position& id) const;
	double GetCost(const Position& from, const Position& to);

private:

	std::vector<std::vector<Tile>> m_Tiles;
	int m_Width = 0;
	int m_Height = 0;
	Vector2D m_MapScale;
};


} // namespace raid