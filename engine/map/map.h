#pragma once

#include "engine/types.h"
#include "engine/map/tile.h"
#include "engine/map/pathfinding.h"

namespace raid
{

class Entity;
class Map
{
public:

	void BuildMap(const int width, const int height);
	void Shutdown();

	// Dimensions
	int GetWidth() const;
	int GetHeight() const;

	// Tiles
	bool IsPositionValid(const int x, const int y) const;
	Tile* GetTile(const int x, const int y);
	Tile* GetTile(const Position& position);
	const Tile* GetTile(const int x, const int y) const;
	const Tile* GetTile(const Position& position) const;
	bool HasTile(const Position& position) const;

	// Tile Availability
	bool CanOccupy(const Position& position) const;
	bool IsOccupied(const Position& position) const;
	bool IsMovementAllowed(const Position& position) const;

	// Scale
	void SetMapScale(const Vector2D& scale) { m_MapScale = scale; }
	const Vector2D& GetMapScale() const { return m_MapScale; }

	// Player Spawn
	void SetPlayerSpawnPosition(const Position& position) { m_PlayerSpawnPosition = position; }
	const Position& GetPlayerSpawnPosition() const { return m_PlayerSpawnPosition; }

	// Pathfinding
	bool BuildPath(Tile* start, Tile* end, TilePath& out_path);
	bool BuildPath(const Position& start, const Position& end, TilePath& out_path);
	std::vector<Position> GetNeighbors(const Position& p) const;
	double GetCost(const Position& from, const Position& to);

	// Finding available spot
	bool GetNearestUnoccupiedTile(const Position& target, Position& out_result);
	bool GetNearestUnoccupiedTile(const Position& target, const Position& from, Position& out_result);

	// Events
	void OnEntityPositionChanged(Entity* entity, const Position& from, const Position& to);

private:

	std::vector<std::vector<Tile>> m_Tiles;
	int m_Width = 0;
	int m_Height = 0;
	Vector2D m_MapScale;
	Position m_PlayerSpawnPosition;
};


} // namespace raid