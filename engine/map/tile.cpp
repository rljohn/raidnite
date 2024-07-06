#include "engine/pch.h"
#include "tile.h"
#include "engine/system/container.h"

namespace raid
{

Tile::Tile()
	: m_Occupant(nullptr)
	, m_Position(-1, -1)
{
}

bool Tile::IsOccupied() const
{
	return m_Occupant != nullptr;
}

void Tile::SetOccupant(Entity* occupant) 
{
	m_Occupant = occupant;
}

void Tile::OnEntityEnter(Entity * e)
{
	m_ActiveEntities.push_back(e);
}

void Tile::OnEntityExit(Entity* e)
{
	VectorRemove(m_ActiveEntities, e);
}

} // namespace raid
