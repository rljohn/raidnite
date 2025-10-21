#pragma once

#include "engine/entity/component.h"
#include "engine/unit/unit_component.h"
#include "engine/unit/movement.h"
#include "engine/unit/targeting.h"

namespace raid
{

class Unit;
class Entity;
class Ability;
struct TilePropertiesChangedEvent;
struct DamageEvent;

struct TargetScanParams
{
	TargetFilter Type = TargetFilter::Any;
	Distance Range = 0;
};

class AIComponent : public UnitComponent
{
public:

	AIComponent(Unit& parent);

	// Core
	void Update(const GameFrame& frame) override;
	void OnGameEvent(const GameEvent& evt) override;

	// Movement
	const Position& GetDesiredPosition() const { return m_DesiredPosition; }
	void SetDesiredPosition(const Position& p);
	bool HasDesiredPosition() const { return m_DesiredPosition != InvalidPosition; }

	// Targeting
	Entity* ScanForTarget(const TargetScanParams& params) const;
	std::vector<Entity*> ScanForTargets(const TargetScanParams& params) const;

	// Casting
	bool IsReadyToCast();
	bool WantsToCastAbility(Ability* a);

protected:

	// Attacking
	void UpdateAggro();

	// Movement
	void BuildPath();

	// Game Events
	void OnMapChanged(const TilePropertiesChangedEvent& evt);
	void OnDamageEvent(const DamageEvent& evt);
	
	// Targeting
	bool CanTargetEntity(const Entity* e, TargetFilter targeting) const;

protected:

	Position m_DesiredPosition;
	MovementComponent& m_Movement;
	Ticker m_AggroTicker;
};

} // namespace raid