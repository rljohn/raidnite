#pragma once

#include "engine/entity/entity.h"
#include "engine/unit/unit.h"
#include "engine/types.h"
#include "engine/engine.h"
#include "engine/game/damage.h"

namespace raid
{

class World : public IEntityManager, public IGameSystem
{

public:

	World();

	// IEntityManager
	Entity* FindEntity(const EntityId entityId) override;
	void OnGameEvent(const GameEvent& e) override;

	// IGameSystem
	void Update(const GameFrame& frame);

	// Registration
	void RegisterEntity(Entity* unit) override;
	void UnRegisterEntity(Entity* unit) override;

	const Entity& GetWorldEntity();

private:

	using EntityList = std::vector<Entity*>;
	EntityList m_Entities;
	Entity m_WorldEntity;

	EntityId m_NextUnitId;
};

// Utility (mostly for testing) to ensure the combat system is set and destroyed.
class WorldRAII
{
public:

	WorldRAII();
	~WorldRAII();

	World Instance;
};

} // namespace World