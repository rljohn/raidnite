#pragma once

#include "engine/entity/entity.h"
#include "engine/system/math/vector2.h"

namespace raid
{

class Tile
{
public:

	Tile();

	// 2 Dimensional Position of the Tile
	const Position& GetPosition() const { return m_Position; }
	void SetPosition(const Position& pos) { m_Position = pos; }

	// An entity "occupies" a tile if it is in control of it.
	// A unit may occupy a tile without residing in it, i.e.
	// is in transit.
	bool IsOccupied() const;
	void SetOccupant(Entity* e);
	Entity* GetOccupant() const { return m_Occupant; }

	// Tiles not in use
	bool IsValid() const { return m_Valid; }
	void SetValid(bool valid) { m_Valid = valid; }

	// Tiles can be enabled/disabled for movement.
	bool AllowsMovement() const { return m_AllowsMovement; }
	void SetMovementEnabled(bool enabled) { m_AllowsMovement = enabled; }

	// Tiles can be enabled/disabled for occupancy
	bool AllowsOccupancy() const { return m_AllowsOccupancy; }
	void SetOccupancyAllowed(bool allowed) { m_AllowsOccupancy = allowed; }

	// Entities may pass through tiles on the way to their occupied tile.
	void OnEntityEnter(Entity* e);
	void OnEntityExit(Entity* e);

	// Return a list for all entities that are currently active on this tile.
	const std::vector<Entity*>& GetActiveEntities()
	{
		return m_ActiveEntities;
	}

private:

	Position m_Position;
	Entity* m_Occupant;
	bool m_Valid;
	bool m_AllowsMovement;
	bool m_AllowsOccupancy;
	std::vector<Entity*> m_ActiveEntities;
};

} // namespace raid