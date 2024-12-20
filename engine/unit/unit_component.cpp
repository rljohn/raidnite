#include "engine/pch.h"
#include "unit_component.h"

#include "engine/unit/unit.h"

namespace raid
{

UnitComponent::UnitComponent(Unit& unit)
	: m_Unit(unit)
	, Component(unit)
{
}

const Position& UnitComponent::GetCurrentPosition() const
{
	return m_Unit.GetTransform().GetPosition();
}

} // namespace raid
