#pragma once

#include "engine/entity/component.h"
#include "engine/unit/unit_component.h"
#include "engine/unit/movement.h"
#include "engine/unit/targeting.h"

namespace raid
{

class Unit;
class Entity;
struct TilePropertiesChangedEvent;

struct TargetScanParams
{
	TargetFilter Type = TargetFilter::Any;
	Distance Range = 0;
};

class AIComponent : public UnitComponent
{
public:

	AIComponent(Unit& parent);

	const Position& GetDesiredPosition() const { return m_DesiredPosition; }
	void SetDesiredPosition(const Position& p);

	void Update(const GameFrame& frame) override;
	void OnGameEvent(const GameEvent& evt) override;

	bool HasDesiredPosition() const { return m_DesiredPosition != InvalidPosition; }

	Entity* ScanForTarget(const TargetScanParams& params) const;
	std::vector<Entity*> ScanForTargets(const TargetScanParams& params) const;

protected:

	void UpdateAggro();

	void BuildPath();
	void OnMapChanged(const TilePropertiesChangedEvent& evt);
	
	bool CanTargetEntity(const Entity* e, TargetFilter targeting) const;

	Position m_DesiredPosition;
	MovementComponent& m_Movement;
	Ticker m_AggroTicker;
};

} // namespace raid