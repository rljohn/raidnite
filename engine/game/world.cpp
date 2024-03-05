#include "engine/pch.h"
#include "world.h"
#include "engine/types.h"
#include "engine/unit/unit.h"

namespace raid
{

World::World()
	: m_NextUnitId(InvalidEntityId)
{
}

Entity* World::FindEntity(EntityId entityId)
{
	for (Entity* e : m_Entities)
	{
		if (e->GetId() == entityId)
			return e;
	}

	return nullptr;
}

void World::OnGameEvent(const GameEvent& evt)
{
	for (Entity* e : m_Entities)
	{
		e->OnGameEvent(evt);
	}
}

void World::Update(const GameFrame& frame)
{
	for (Entity* e : m_Entities)
	{
		
	}
}

void World::RegisterEntity(Entity* unit)
{
	m_Entities.push_back(unit);
	unit->SetId(++m_NextUnitId);
	unit->Init();
}

void World::UnRegisterEntity(Entity* unit)
{
	VectorRemove<Entity*>(m_Entities, unit);
}

const Entity& World::GetWorldEntity()
{
	return m_WorldEntity;
}

WorldRAII::WorldRAII()
{
	Game::SetEntityManager(&Instance);
}

WorldRAII::~WorldRAII()
{
	Game::SetEntityManager(nullptr);
}

} // namespace raid