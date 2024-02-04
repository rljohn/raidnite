#pragma once

#include "engine/game/combat_common.h"
#include "engine/game/damage.h"

namespace raid
{

enum class CombatEventType
{
	Invalid = 0,
	CombatStart,
	CombatEnd,
	Damage
};

struct CombatEvent
{
	virtual CombatEventType GetType() const = 0;
};

struct CombatStartEvent : public CombatEvent
{

};

struct CombatEndEvent : public CombatEvent
{

};

struct DamageEvent : public CombatEvent
{
	CombatEventType GetType() const override { return CombatEventType::Damage; }

	float BaseDamage = 0;
	float ActualDamage = 0;
	DamageType DamageType = DamageType::Invalid;
};

} // namespace raid