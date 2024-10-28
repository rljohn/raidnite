#pragma once

#include "engine/entity/entity.h"
#include "engine/game/faction.h"

namespace raid
{

class Map;

class IEntitySpawner
{
public:

	virtual Entity* SpawnEntity(Map* map, const Position& pos) = 0;
	virtual void DestroyEntity(Map* map, Entity* entity) = 0;
};

class UnitSpawner : public IEntitySpawner, public Entity
{
public:

	Entity* SpawnEntity(Map* map, const Position& pos) override;
	void DestroyEntity(Map* map, Entity* entity) override;
};

} // namespace raid