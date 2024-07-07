#pragma once

#include "engine/entity/entity.h"

namespace raid
{

class Map;

class IEntitySpawner
{
public:

	virtual Entity* SpawnEntity(Map* map, const Position& pos) = 0;
};

class UnitSpawner : public IEntitySpawner, public Entity
{
public:

	Entity* SpawnEntity(Map* map, const Position& pos) override;
};

} // namespace raid