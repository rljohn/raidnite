#include "engine/pch.h"
#include "entity.h"
#include "engine/entity/component.h"

namespace raid
{

Entity::Entity()
	: m_Id(InvalidEntityId)
{
}

Entity::~Entity()
{
	for (auto& pair : m_Components)
	{
		pair.second->Shutdown();
		delete pair.second;
	}

	m_Components.clear();
}

void Entity::Init()
{
	for (auto& pair : m_Components)
	{
		pair.second->Init();
	}

	OnInit();
}

void Entity::Update(const GameFrame& frame)
{
	for (auto& pair : m_Components)
	{
		pair.second->Update(frame);
	}
}

void Entity::OnGameEvent(const GameEvent& evt)
{
	for (auto& pair : m_Components)
	{
		pair.second->OnGameEvent(evt);
	}
}

} // namespace raid