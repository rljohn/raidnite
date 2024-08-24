#include "engine/pch.h"
#include "ai.h"

#include "engine/game/game_events.h"
#include "engine/map/map.h"
#include "engine/map/pathfinding.h"
#include "engine/system/math/vector_math.h"
#include "engine/unit/unit.h"

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

void AIComponent::OnGameEvent(const GameEvent& evt)
{
	switch (evt.GetType())
	{
	case GameEventType::TilePropertiesChanged:
	{
		const TilePropertiesChangedEvent& e = static_cast<const TilePropertiesChangedEvent&>(evt);
		OnMapChanged(e);
		break;
	}
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

	map->SetTileOccupation(path.GetDestination()->GetPosition(), GetParent(), m_Unit.GetTransform());
}

void AIComponent::OnMapChanged(const TilePropertiesChangedEvent& evt)
{
	Map* map = Game::GetMap();
	if (!map)
		return;
	
	bool findNewPosition = false;
	
	const Position& pos = evt.GetPosition();

	bool isDestination = (pos == m_Unit.GetTransform().GetOccupyingTile());
	bool isCurrent = (pos == m_Unit.GetTransform().GetPosition());
	bool isMoving = (m_Movement.GetPath().length() > 0);

	bool rebuildPath = isMoving;

	if (Tile* t = map->GetTile(pos))
	{
		if (isDestination && !t->AllowsOccupancy())
		{
			findNewPosition = true;
			rebuildPath = true;
		}	
	}
	else if (isCurrent)
	{
		// Our current tile no longer exists, snap us somewhere nearby.
		raid::Position warp;
		if (map->GetNearestUnoccupiedTile(m_Unit.GetTransform().GetPosition(), warp))
		{
			m_Movement.Warp(warp);

			if (isMoving)
			{
				findNewPosition = true;
			}
		}
		else
		{
			unitError("Could not warp unit to nearby location");
			
			// TODO - Kill a unit that cannot be warped?
		}
	}

	if (findNewPosition)
	{
		raid::Position dest;
		if (map->GetNearestUnoccupiedTile(m_DesiredPosition, dest))
		{
			SetDesiredPosition(dest);
		}
	}

	if (rebuildPath)
	{
		BuildPath();
	}
}

} // namespace raid