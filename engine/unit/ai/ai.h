#pragma once

#include "engine/entity/component.h"
#include "engine/unit/unit_component.h"

namespace raid
{

class Unit;
class Entity;

class AIComponent : public UnitComponent
{
public:

	AIComponent(Unit& parent);

	const Position& GetDesiredPosition() const { return m_DesiredPosition; }
	void SetDesiredPosition(const Position& p) { m_DesiredPosition = p; }

	Position m_DesiredPosition;
};

} // namespace raid