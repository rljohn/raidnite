#pragma once

#include "engine/game/game.h"
#include "engine/game/combat_common.h"
#include "engine/game/damage.h"
#include "engine/entity/entity.h"

namespace raid
{

struct GameStartEvent : public GameEvent
{
	virtual GameEventType GetType() const { return GameEventType::GameStart; }
};

struct GameEndEvent : public GameEvent
{
	virtual GameEventType GetType() const { return GameEventType::GameEnd; }
};

struct ZoneEnterEvent : public GameEvent
{
	virtual GameEventType GetType() const { return GameEventType::ZoneEnter; }
};

struct ZoneExitEvent : public GameEvent
{
	virtual GameEventType GetType() const { return GameEventType::ZoneExit; }
};

struct CombatStartEvent : public GameEvent
{
	virtual GameEventType GetType() const { return GameEventType::CombatStart; }
};

struct CombatEndEvent : public GameEvent
{
	virtual GameEventType GetType() const { return GameEventType::CombatEnd; }
};

struct EntityEvent
{
	EntityEvent() = delete;
	EntityEvent(Entity* e)
		: m_Entity(e)
	{
	}

	Entity* GetEntity() const { return m_Entity; }

protected:

	Entity* m_Entity;
};

struct DeathEvent : public GameEvent, EntityEvent
{
	virtual GameEventType GetType() const { return GameEventType::Death; }

	DeathEvent(Entity* e)
		: EntityEvent(e)
	{
	}
};

struct DamageEvent : public GameEvent, EntityEvent
{
	GameEventType GetType() const override { return GameEventType::Damage; }

	DamageEvent(Entity* e)
		: EntityEvent(e)
	{
	}

	float BaseDamage = 0;
	float ActualDamage = 0;
	DamageType DamageType = DamageType::Invalid;
};

} // namespace raid