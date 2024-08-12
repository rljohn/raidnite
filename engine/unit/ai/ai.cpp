#include "engine/pch.h"
#include "ai.h"

#include "engine/unit/unit.h"
#include "engine/map/map.h"
#include "engine/map/pathfinding.h"
#include "engine/system/math/vector_math.h"

namespace raid
{

AIComponent::AIComponent(Unit& parent)
	: UnitComponent(parent)
	, m_Movement(*parent.GetComponent<MovementComponent>())
{

}

void AIComponent::SetDesiredPosition(const Position& p)
{ 
	if (m_DesiredPosition != p)
	{
		m_DesiredPosition = p;
		BuildPath();
	}
}

void AIComponent::BuildPath()
{
	Map* map = Game::GetMap();
	if (!map)
		return;

	const Position& current = m_Unit.GetTransform().GetPosition();

	TilePath path;
	if (map->BuildPath(current, m_DesiredPosition, path))
	{
		m_Movement.SetPath(path);
	}

	Tile* tile = path.GetDestination();
	if (tile)
	{
		const Position& previous = m_Unit.GetTransform().GetOccupyingTile();
		if (map->IsPositionValid(previous.GetX(), previous.GetY()))
		{
			if (Tile* prevTile = map->GetTile(previous))
			{
				prevTile->SetOccupant(nullptr);
			}
		}

		tile->SetOccupant(GetParent());
		m_Unit.GetTransform().SetOccupyingTile(tile->GetPosition());
	}
}

} // namespace raid