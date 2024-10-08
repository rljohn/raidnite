#include "engine/pch.h"
#include "spawner.h"
#include "unit.h"
#include "engine/game/game_events.h"
#include "engine/map/map.h"
#include "engine/system/log/logging.h"

namespace raid
{

Entity* UnitSpawner::SpawnEntity(Map* map, const Position& pos)
{
	if (Unit* unit = new Unit())
	{
		unit->GetStateMachine().SetState(StateType::Idle);
		unit->GetTransform().SetPosition(pos);

		// Setup attributes
		unit->GetAttributes().SetupAttributes();

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
				tile->OnEntityEnter(unit);
				map->SetTileOccupation(pos, unit, unit->GetTransform());
			}
			else
			{
				mapError("Unable to spawn entity at %d,%dd : no tile", pos.GetX(), pos.GetY());
			}
		}
		
		EntitySpawnedEvent e(unit);
		Game::DispatchGameEvent(e);

		return unit;
	}

	return nullptr;
}

void UnitSpawner::DestroyEntity(Map* map, Entity* entity)
{
	if (map)
	{
		if (TransformComponent* t = entity->GetComponent<TransformComponent>())
		{
			const Position& p = t->GetOccupyingTile();
			if (Tile* tile = map->GetTile(p))
			{
				mapAssert(tile->GetOccupant() == entity);
				if (tile->GetOccupant() == entity)
				{
					tile->SetOccupant(nullptr);
				}
			}
		}
	}

	if (Game::GetEntityManager())
	{
		Game::GetEntityManager()->UnRegisterEntity(entity);
	}

	EntityDestroyedEvent e(entity);
	Game::DispatchGameEvent(e);

	delete entity;
}

} // namespace raid