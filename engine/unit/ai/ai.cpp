#include "engine/pch.h"
#include "ai.h"

#include "engine/game/ability.h"
#include "engine/game/faction.h"
#include "engine/game/game_events.h"
#include "engine/map/map.h"
#include "engine/map/pathfinding.h"
#include "engine/system/check.h"
#include "engine/system/math/vector_math.h"
#include "engine/unit/unit.h"
#include "engine/unit/unit_utils.h"

namespace raid
{

AIComponent::AIComponent(Unit& parent)
	: UnitComponent(parent)
	, m_Movement(*parent.GetComponent<MovementComponent>())
	, m_DesiredPosition(InvalidPosition)
{
	m_AggroTicker.Init(Milliseconds(100), [this]() { UpdateAggro(); });
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

void AIComponent::Update(const GameFrame& frame)
{
	m_AggroTicker.Update(frame.TimeStep);
}

void AIComponent::UpdateAggro()
{
	if (m_Unit.IsInCombat())
		return;

	if (!m_Unit.GetAggro().IsHostile())
		return;

	TargetScanParams params;
	params.Type = TargetFilter::Enemy;
	params.Range = m_Unit.GetAggro().GetAggroRange();

	std::vector<Entity*> list = ScanForTargets(params);
	if (!list.empty())
	{
		Entity* target = list[0];
		m_Unit.GetAggro().AddUnit(target);
		m_Unit.GetTargeting().SetTarget(target);
	}
}

void AIComponent::OnGameEvent(const GameEvent& evt)
{
	switch (evt.GetType())
	{
	case GameEventType::Damage:
	{
		const DamageEvent& dmg = static_cast<const DamageEvent&>(evt);
		OnDamageEvent(dmg);
	}
	break;
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

void AIComponent::OnDamageEvent(const DamageEvent& dmg)
{
	// Already in combat, early out
	if (m_Unit.IsInCombat())
		return;

	Entity* addTarget = nullptr;

	// Outgoing damage
	if (dmg.Source == GetParent())
	{
		addTarget = dmg.GetEntity();
	}
	// Incoming Damage
	else if (dmg.GetEntity() == GetParent())
	{
		addTarget = dmg.GetSource();
	}

	// Try to add target?
	if (addTarget)
	{
		// Can we target this entity?
		if (CanTargetEntity(addTarget, TargetFilter::Enemy))
		{
			// Add unit to aggro table
			m_Unit.GetAggro().AddUnit(addTarget);

			// Target if not targetting anything
			if (!m_Unit.GetTargeting().HasTarget())
			{
				m_Unit.GetTargeting().SetTarget(addTarget);
			}
		}
	}
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

bool AIComponent::CanTargetEntity(const Entity* e, TargetFilter targeting) const
{
	stry
	{
		IFactionManager* factions = Game::GetFactionManager();
		scheckall(factions);

		FactionId myFactionId = InvalidFactionId;
		scheckall(UnitStatics::GetAttribute<AttributeType::Faction>(m_Parent, myFactionId));

		FactionId targetFaction = InvalidFactionId;
		scheckall(UnitStatics::GetAttribute<AttributeType::Faction>(*e, targetFaction));

		if (targeting == TargetFilter::Enemy)
		{
			return factions->IsNeutralOrHostile(myFactionId, targetFaction);
		}
		else if (targeting == TargetFilter::Friendly)
		{
			return factions->IsFriendly(myFactionId, targetFaction);
		}
	}
	scatchall
	{
	}

	return false;
}

Entity* AIComponent::ScanForTarget(const TargetScanParams& params) const
{
	IEntityManager* entityMgr = Game::GetEntityManager();
	if (!entityMgr)
		return nullptr;

	Entity* target = nullptr;

	entityMgr->ForEach([&](Entity* e)
	{
		// Scan should not include self
		if (e == &m_Parent)
			return false;

		if (!CanTargetEntity(e, params.Type))
		{
			return false;
		}

		if (!UnitStatics::IsEntityInRange(m_Unit, *e, params.Range))
			return false;

		target = e;
		return true;
		
	});

	return target;
}

std::vector<Entity*> AIComponent::ScanForTargets(const TargetScanParams& params) const
{
	std::vector<Entity*> results;

	stry
	{
		IEntityManager* entityMgr = Game::GetEntityManager();
		scheckall(entityMgr);

		entityMgr->ForEach([&](Entity* e)
		{
			if (!CanTargetEntity(e, TargetFilter::Enemy))
				return false;

			if (!UnitStatics::IsEntityInRange(m_Unit, *e, params.Range))
				return false;

			results.push_back(e);
			return false;
		});
	}
	scatchall
	{
		results.clear();
	}
	
	return results;
}

bool AIComponent::IsReadyToCast()
{
	AbilityComponent& abilities = m_Unit.GetAbilityComponent();

	for (Ability* ability : abilities)
	{
		
	}

	// TODO
	//  Enumerate abilities
	//  Find any ability that is ready to cast

	return false;
}

} // namespace raid