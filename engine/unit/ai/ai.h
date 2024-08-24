#pragma once

#include "engine/entity/component.h"
#include "engine/unit/unit_component.h"
#include "engine/unit/movement.h"

namespace raid
{

class Unit;
class Entity;
struct TilePropertiesChangedEvent;

class AIComponent : public UnitComponent
{
public:

	AIComponent(Unit& parent);

	const Position& GetDesiredPosition() const { return m_DesiredPosition; }
	void SetDesiredPosition(const Position& p);

	void OnGameEvent(const GameEvent& evt) override;

protected:

	void BuildPath();
	void OnMapChanged(const TilePropertiesChangedEvent& evt);

	Position m_DesiredPosition;
	MovementComponent& m_Movement;
};

} // namespace raid