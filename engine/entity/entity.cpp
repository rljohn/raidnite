#include "engine/pch.h"
#include "entity.h"
#include "engine/entity/component.h"

namespace raid
{

Entity::Entity()
	: m_Id(InvalidEntityId)
{
}

void Entity::Init()
{
	for (auto& pair : m_Components)
	{
		pair.second->Init();
	}

	OnInit();
}

void Entity::OnGameEvent(const GameEvent& evt)
{
	for (auto& pair : m_Components)
	{
		pair.second->OnGameEvent(evt);
	}
}

} // namespace raid