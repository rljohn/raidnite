#include "engine/pch.h"
#include "world.h"
#include "engine/types.h"
#include "engine/unit/unit.h"

namespace raid
{

World::World()
{

}

void World::RegisterUnit(Unit* unit)
{
	m_Units.push_back(unit);
}

void World::UnregisterUnit(Unit* unit)
{
	RemoveFromVector<Unit*>(m_Units, unit);
}

World::UnitList& World::GetUnitList()
{
	return m_Units;
}

const Entity& World::GetWorldEntity()
{
	return m_WorldEntity;
}

} // namespace raid