#pragma once

#include "engine/entity/entity.h"
#include "engine/unit/unit.h"
#include "engine/types.h"
#include "engine/engine.h"
#include "engine/game/damage.h"

namespace raid
{

class World : public IEntityManager, public IGameSystem, public IGameEventListener
{

public:

	World();

	// IEntityManager
	Entity* FindEntity(const EntityId entityId) override;
	void Reset() override;
	size_t GetEntityCount() const override;
	Entity* GetEntity(size_t idx) override;
	void AddEntityListener(IEntityListener* listener) override;
	void RemoveEntityListener(IEntityListener* listener) override;

	// IGameSystem
	void Update(const GameFrame& frame);

	// IGameEventListener
	void OnGameEvent(const GameEvent& e) override;

	// Registration
	void RegisterEntity(Entity* unit) override;
	void UnRegisterEntity(Entity* unit) override;

private:

	using EntityList = std::vector<Entity*>;
	EntityList m_Entities;
	EntityId m_NextUnitId;

	using Listeners = std::vector<IEntityListener*>;
	Listeners m_Listeners;
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