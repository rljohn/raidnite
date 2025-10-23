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
	
	Unit& GetUnit() { return m_Unit; }
	const Unit& GetUnit() const { return m_Unit; }

protected:

	Unit& m_Unit;
};

}