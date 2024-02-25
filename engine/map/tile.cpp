#include "engine/pch.h"
#include "tile.h"

namespace raid
{

bool Tile::IsOccupied() const
{
	return m_Occupant != nullptr;
}

} // namespace raid
