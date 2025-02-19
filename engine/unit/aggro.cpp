#include "engine/pch.h"
#include "engine/unit/aggro.h"

#include "engine/system/log/log_channels.h"
#include "engine/game/combat.h"
#include "engine/system/check.h"

namespace raid
{

void AggroComponent::AddUnit(Entity* unit)
{
	unitVerbose("[{}] Add Aggro: {}", m_Parent.GetLogName(), unit->GetLogName());
	m_List.push_back(unit);
}

void AggroComponent::RemoveUnit(Entity* unit)
{
	unitVerbose("[{}] Remove Aggro: {}", m_Parent.GetLogName(), unit->GetLogName());
	m_List.remove(unit);
}

void AggroComponent::OnGameEvent(const GameEvent& evt)
{
	switch (evt.GetType())
	{
	case GameEventType::Death:
		const DeathEvent& d = static_cast<const DeathEvent&>(evt);
		OnUnitDied(d);
		break;
	}
}

void AggroComponent::OnUnitDied(const DeathEvent& d)
{
	if (Entity* e = d.GetEntity())
	{
		RemoveUnit(e);
	}
}

} // namespace raid