#pragma once

#include "engine/entity/component.h"

// Components that can only be added to units

namespace raid
{

class Unit;

class UnitComponent : public Component
{
public:

	UnitComponent(Unit& unit);

	const Position& GetCurrentPosition() const;

protected:

	Unit& m_Unit;
};

}