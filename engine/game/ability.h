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
};

class Ability
{
public:

	Ability(const AbilityDefinition def)
		: m_Definition(def)
	{

	}

	AbilityId GetId() const { return m_Definition.Id; }
	PowerType GetCostType() const { return m_Definition.CostType; }
	double GetCostValue() const { return m_Definition.CostValue; }
	double GetRange() const { return m_Definition.Range; }
	bool CanCastWhileMoving() const { return m_Definition.CastWhileMoving; }
	double GetDamage() const { return m_Definition.Damage; }

private:

	AbilityDefinition m_Definition;
};

class AbilityComponent : public Component
{
public:

	AbilityComponent(Entity& parent)
		: Component(parent)
	{
	}

	void AddAbility(const AbilityDefinition& def);
	Ability* GetAbility(AbilityId spell);

private:

	std::vector<Ability*> m_Abilities;
};

}