#include "engine/pch.h"
#include "engine/unit/aggro.h"

#include "engine/game/combat.h"
#include "engine/system/check.h"

namespace raid
{

void AggroTable::AddUnit(Entity* unit)
{
	m_List.push_back(unit);
}

void AggroTable::RemoveUnit(Entity* unit)
{
	m_List.remove(unit);
}

void AggroTable::OnGameEvent(const GameEvent& evt)
{
	switch (evt.GetType())
	{
	case GameEventType::Death:
		const DeathEvent& d = static_cast<const DeathEvent&>(evt);
		OnUnitDied(d);
		break;
	}
}

void AggroTable::OnUnitDied(const DeathEvent& d)
{
	if (Entity* e = d.GetEntity())
	{
		RemoveUnit(e);
	}
}

} // namespace raid