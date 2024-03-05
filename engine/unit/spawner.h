#pragma once

namespace raid
{

class IEntitySpawner
{
public:

	virtual Entity* SpawnEntity() = 0;
};

class UnitSpawner : public IEntitySpawner
{
public:

	Entity* SpawnEntity() override;
};

} // namespace raid