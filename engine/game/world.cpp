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

void World::Reset()
{
	while (!m_Entities.empty())
	{
		Entity* e = m_Entities.back();
		m_Entities.pop_back();
		delete e;
	}
	
	m_Entities.clear();
	m_NextUnitId = InvalidEntityId;
}

size_t World::GetEntityCount() const
{
	return m_Entities.size();
}

Entity* World::GetEntity(size_t idx)
{
	if (idx < m_Entities.size())
	{
		return m_Entities[idx];
	}

	return nullptr;
}

void World::AddEntityListener(IEntityListener* listener)
{
	m_Listeners.push_back(listener);
}

void World::RemoveEntityListener(IEntityListener* listener)
{
	VectorRemove(m_Listeners, listener);
}

void World::Update(const GameFrame& frame)
{
	for (Entity* e : m_Entities)
	{
		e->Update(frame);
	}
}

void World::RegisterEntity(Entity* unit)
{
	m_Entities.push_back(unit);

	EntityId nextId = ++m_NextUnitId;
	if (nextId == InvalidEntityId)
		nextId = ++m_NextUnitId;

	unit->SetId(nextId);
	unit->Init();
}

void World::UnRegisterEntity(Entity* unit)
{
	VectorRemove<Entity*>(m_Entities, unit);

	for (IEntityListener* listener : m_Listeners)
	{
		listener->OnEntityRemoved(unit);
	}
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