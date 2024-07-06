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
	std::vector<Entity*> m_ActiveEntities;
};

} // namespace raid