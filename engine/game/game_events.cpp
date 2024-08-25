#include "engine/pch.h"
#include "game_events.h"

#include "engine/map/tile.h"

namespace raid
{

TilePropertiesChangedEvent::TilePropertiesChangedEvent(const Tile* tile)
	: m_Position(tile->GetPosition())
	, m_IsValid(tile->IsValid())
	, m_AllowsMovement(tile->AllowsMovement())
	, m_AllowsOccupancy(tile->AllowsOccupancy())
{

}

}
