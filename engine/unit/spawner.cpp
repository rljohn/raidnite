#include "engine/pch.h"
#include "spawner.h"
#include "unit.h"
#include "engine/game/game_events.h"
#include "engine/map/map.h"

namespace raid
{

Entity* UnitSpawner::SpawnEntity(Map* map, const Position& pos)
{
	if (Unit* unit = new Unit())
	{
		unit->GetTransform().SetPosition(pos);

		// Every spawned unit must have a Health attribute.
		unit->AddPower<PowerType::Health>(0);

		// Units must be registered with the world's entity manager.
		if (Game::GetEntityManager())
		{
			Game::GetEntityManager()->RegisterEntity(unit);
		}
		
		if (map)
		{
			if (Tile* tile = map->GetTile(pos))
			{
				tile->SetOccupant(unit);
			}
			else
			{
				mapError("Unable to spawn entity at %d,%dd : no tile", pos.GetX(), pos.GetY());
			}
		}
		
		UnitSpawnedEvent e(unit);
		Game::DispatchGameEvent(&e);

		return unit;
	}

	return nullptr;
}

} // namespace raid