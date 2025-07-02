#pragma once

#include "engine/entity/component.h"
#include "engine/unit/Power.h"

#include <vector>

namespace raid
{

struct AbilityDefinition
{
	AbilityId Id;
	PowerType CostType;
	double CostValue;
	double Range;
	bool CastWhileMoving;
	double Damage;
	double Cooldown;
};

class Ability
{
public:

	Ability(const AbilityDefinition def)
		: m_Definition(def)
		, m_LastCastFrame(0)
	{

	}

	AbilityId GetId() const { return m_Definition.Id; }
	PowerType GetCostType() const { return m_Definition.CostType; }
	double GetCostValue() const { return m_Definition.CostValue; }
	double GetRange() const { return m_Definition.Range; }
	bool CanCastWhileMoving() const { return m_Definition.CastWhileMoving; }
	double GetDamage() const { return m_Definition.Damage; }
	double GetBaseCooldown() const { return m_Definition.Cooldown; }

private:

	AbilityDefinition m_Definition;
	Frame m_LastCastFrame;
};

class AbilityComponent : public Component
{
public:

	AbilityComponent(Entity& parent)
		: Component(parent)
		, m_CurrentAbility(nullptr)
	{
	}

	void AddAbility(const AbilityDefinition& def);
	Ability* GetAbility(AbilityId spell);

	void SetCurrentAbility(Ability* ability);
	Ability* GetCurrentAbility() const { return m_CurrentAbility; }

private:

	std::vector<Ability*> m_Abilities;
	Ability* m_CurrentAbility = nullptr;
};

}