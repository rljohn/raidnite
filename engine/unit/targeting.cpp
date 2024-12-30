#include "engine/pch.h"
#include "targeting.h"

#include "engine/game/game_events.h"

namespace raid
{

bool TargetingComponent::HasTarget() const
{
	return m_Target != nullptr;
}

Entity* TargetingComponent::GetTarget() const
{
	return m_Target;
}

void TargetingComponent::SetTarget(Entity* entity)
{
	if (m_Target != entity)
	{
		m_Target = entity;

		TargetChangedEvent evt(&m_Parent, entity);
		Game::DispatchGameEvent(evt);
	}
}

} // namespace raid