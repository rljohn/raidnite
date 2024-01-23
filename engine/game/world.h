#pragma once

#include "engine/entity/entity.h"
#include "engine/unit/unit.h"
#include "engine/types.h"

namespace raid
{

class World
{

public:

	World();

	void RegisterUnit(Unit* unit);
	void UnregisterUnit(Unit* unit);

	using UnitList = std::vector<Unit*>;
	UnitList& GetUnitList();

	const Entity& GetWorldEntity();

private:

	UnitList m_Units;
	Entity m_WorldEntity;

};

} // namespace World