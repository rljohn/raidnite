#pragma once

#include "engine/entity/component.h"
#include "engine/system/bit.h"
#include "engine/system/container.h"
#include "engine/unit/power.h"

#include <vector>

namespace raid
{

enum class AbilityTargetingFlags
{
	Self = BIT(0),
	Ground = BIT(1),
	EnemyTarget = BIT(2),
	Friendly = BIT(3)
};
DECLARE_FLAG_OPERATORS(AbilityTargetingFlags);

struct AbilityDefinition
{
	AbilityId Id;
	PowerType CostType;
	double CostValue;
	double Range;
	bool CastWhileMoving;
	AbilityTargetingFlags Targeting;
	double Damage;
	double CooldownSeconds;
	bool UseCcd;
};

class Ability
{
public:

	Ability(const AbilityDefinition def)
		: m_Definition(def)
		, m_LastCastFrame(0)
		, m_NextCastFrame(0)
	{

	}

	// Attributes
	AbilityId GetId() const { return m_Definition.Id; }
	PowerType GetCostType() const { return m_Definition.CostType; }
	double GetCostValue() const { return m_Definition.CostValue; }
	double GetRange() const { return m_Definition.Range; }
	bool CanCastWhileMoving() const { return m_Definition.CastWhileMoving; }
	double GetDamage() const { return m_Definition.Damage; }
	double GetBaseCooldown() const { return m_Definition.CooldownSeconds; }
	bool UseGlobalCooldown() const { return m_Definition.UseCcd; }

	// Events
	void OnCast(Frame frame, double cooldownReduction);

	// Queries
	bool IsOnCooldown() const;
	bool IsSelfCast() const;
	bool IsFriendlyCast() const;
	bool IsGroundCast() const;
	bool IsEnemyCast() const;

	bool IsAvailable() const;

private:

	AbilityDefinition m_Definition;
	Frame m_LastCastFrame;
	Frame m_NextCastFrame;
};

class AbilityComponent : public Component
{
public:

	AbilityComponent(Entity& parent)
		: Component(parent)
		, m_CurrentAbility(nullptr)
		, m_LastGcdFrame(0)
		, m_NextGcdFrame(0)
	{
	}

	void AddAbility(const AbilityDefinition& def);
	Ability* GetAbility(AbilityId spell);

	void SetCurrentAbility(Ability* ability);
	Ability* GetCurrentAbility() const { return m_CurrentAbility; }

	void OnCast(Frame frame);

	bool IsOnGlobalCooldown() const;
	bool StartGlobalCooldown(Frame frame, double cooldownReduction);
	double GetBaseGlobalCooldown() { return 1.0; }

private:

	std::vector<Ability*> m_Abilities;
	DECLARE_ITERABLE(m_Abilities);

	Ability* m_CurrentAbility = nullptr;

	// Global Cooldown
	Frame m_LastGcdFrame;
	Frame m_NextGcdFrame;
};

}