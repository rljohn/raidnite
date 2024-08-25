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
	, m_DesiredPosition(InvalidPosition)
{

}

void AIComponent::SetDesiredPosition(const Position& p)
{ 
	if (m_DesiredPosition != p)
	{
		m_DesiredPosition = p;

		if (m_DesiredPosition != m_Unit.GetTransform().GetOccupyingTile())
		{
			BuildPath();
		}
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
	if (m_DesiredPosition == InvalidPosition)
	{
		unitError("Tried to build path to invalid position");
		return;
	}

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
	
	const Position& pos = evt.GetPosition();
	Tile* t = map->GetTile(pos);
	if (!t)
		return;

	Position currentOccupyingPos = m_Unit.GetTransform().GetOccupyingTile();

	bool findNewPosition = false;
	bool isDestination = (pos == currentOccupyingPos);
	bool isCurrent = (pos == m_Unit.GetTransform().GetPosition());
	bool isMoving = (m_Movement.GetPath().length() > 0);
	bool rebuildPath = isMoving;

	// Check if we must find a new destination
	if (t->IsValid())
	{
		if (isDestination && !t->AllowsOccupancy())
		{
			findNewPosition = true;
			rebuildPath = true;
		}	
	}
	
	// If the tile is the one we're currently standing on.
	bool canBeOnTile = t->IsValid() && t->AllowsMovement();
	if (isCurrent && !canBeOnTile)
	{
		// Our current tile no longer exists, snap us somewhere nearby.
		raid::Position warp;
		if (map->GetNearestMoveableTile(m_Unit.GetTransform().GetPosition(), warp))
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

	// We need a new position near our previous destination.
	if (findNewPosition)
	{
		raid::Position desire =
			HasDesiredPosition() ? m_DesiredPosition : currentOccupyingPos;

		raid::Position dest;
		if (map->GetNearestUnoccupiedTile(desire, dest))
		{
			m_DesiredPosition = dest;
		}
		else
		{
			m_DesiredPosition = InvalidPosition;
			m_Movement.ResetPath();
			rebuildPath = false;
		}
	}

	// Build a new path to our destination
	if (rebuildPath)
	{
		BuildPath();
	}
}

} // namespace raid