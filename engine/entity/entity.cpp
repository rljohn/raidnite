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
}

} // namespace raid