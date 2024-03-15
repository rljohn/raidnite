#pragma once

#include "engine/entity/entity.h"

namespace raid
{

class IEntitySpawner
{
public:

	virtual Entity* SpawnEntity() = 0;
};

class UnitSpawner : public IEntitySpawner, public Entity
{
public:

	Entity* SpawnEntity() override;
};

} // namespace raid