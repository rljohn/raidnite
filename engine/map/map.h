#pragma once

#include "engine/game/game.h"
#include "engine/types.h"
#include "engine/map/tile.h"
#include "engine/map/pathfinding.h"

namespace raid
{

class Entity;
class TransformComponent;

class Map : public IGameEventListener
{
public:

	void BuildMap(const PositionScalar width, const PositionScalar height);
	void Shutdown();

	// Events
	void RegisterForEvents();
	void OnGameEvent(const GameEvent& evt);

	// Dimensions
	PositionScalar GetWidth() const;
	PositionScalar GetHeight() const;

	// Tiles
	bool IsPositionValid(const PositionScalar x, const PositionScalar y) const;
	bool IsPositionValid(const Position& pos) const;
	Tile* GetTile(const PositionScalar x, const PositionScalar y);
	Tile* GetTile(const Position& position);
	const Tile* GetTile(const PositionScalar x, const PositionScalar y) const;
	const Tile* GetTile(const Position& position) const;

	// Tile Validity
	bool HasTile(const Position& position) const;
	bool IsTileEnabled(const Position& position) const;
	void SetTileEnabled(const Position& position, bool enabled);

	// Tile Occupancy
	bool CanOccupy(const Position& position) const;
	bool IsOccupied(const Position& position) const;
	void SetTileOccupation(const Position& pos, Entity* entity, TransformComponent& transform);
	void SetOccupancyAllowed(const Position& position, bool allow);

	// Tile Movement
	bool IsMovementAllowed(const Position& position) const;
	void SetMovementAllowed(const Position& position, bool allow);

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

	// Finding valid tiles
	bool GetNearestMoveableTile(const Position& target, Position& out_result);
	bool GetNearestMoveableTile(const Position& target, const Position& from, Position& out_result);

private:

	void OnEntityPositionChanged(Entity* entity, const Position& from, const Position& to);

	std::vector<std::vector<Tile>> m_Tiles;
	PositionScalar m_Width = 0;
	PositionScalar m_Height = 0;
	Vector2D m_MapScale;
	Position m_PlayerSpawnPosition;
};


} // namespace raid