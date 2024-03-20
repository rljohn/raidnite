#pragma once

#include "engine/entity/entity.h"

namespace raid
{

class IEntitySpawner
{
public:

	virtual Entity* SpawnEntity(const Position& pos) = 0;
};

class UnitSpawner : public IEntitySpawner, public Entity
{
public:

	Entity* SpawnEntity(const Position& pos) override;
};

} // namespace raid