#pragma once

#include "engine/game/combat_common.h"
#include "engine/game/damage.h"
#include "engine/entity/entity.h"

namespace raid
{

enum class GameEventType
{
	Invalid = 0,
	CombatStart,
	CombatEnd,
	Damage,
	Death
};

struct GameEvent
{
	virtual GameEventType GetType() const = 0;
};

struct CombatStartEvent : public GameEvent
{
	virtual GameEventType GetType() const { return GameEventType::CombatStart; }
};

struct CombatEndEvent : public GameEvent
{
	virtual GameEventType GetType() const { return GameEventType::CombatEnd; }
};

struct DeathEvent : public GameEvent
{
	virtual GameEventType GetType() const { return GameEventType::Death; }

	DeathEvent() = delete;
	DeathEvent(Entity* unit)
		: m_Entity(unit)
	{
	}

	Entity* GetEntity() const { return m_Entity; }

private:

	Entity* m_Entity;
};

struct DamageEvent : public GameEvent
{
	GameEventType GetType() const override { return GameEventType::Damage; }

	float BaseDamage = 0;
	float ActualDamage = 0;
	DamageType DamageType = DamageType::Invalid;
};

} // namespace raid