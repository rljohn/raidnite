#include "engine/pch.h"
#include "spawner.h"
#include "unit.h"

namespace raid
{

Entity* UnitSpawner::SpawnEntity()
{
	if (Unit* unit = new Unit())
	{
		// Every spawned unit must have a Health attribute.
		unit->AddPower<PowerType::Health>(0);

		// Units must be registered with the world's entity manager.
		if (Game::GetEntityManager())
		{
			Game::GetEntityManager()->RegisterEntity(unit);
		}
		
		return unit;
	}

	return nullptr;
}

} // namespace raid