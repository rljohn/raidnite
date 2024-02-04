#pragma once

#include "engine/entity/entity.h"
#include "engine/unit/unit.h"
#include "engine/types.h"
#include "engine/game/damage.h"

namespace raid
{

class World : public IEntityLocator
{

public:

	World();

	// IEntityLocator
	Entity* FindEntity(const EntityId entityId) override;

	// Registration
	void RegisterEntity(Entity* unit);
	void UnRegisterEntity(Entity* unit);

	const Entity& GetWorldEntity();

private:

	using EntityList = std::vector<Entity*>;
	EntityList m_Entities;
	Entity m_WorldEntity;

	EntityId m_NextUnitId;
};

} // namespace World