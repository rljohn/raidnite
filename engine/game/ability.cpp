#include "engine/pch.h"
#include "ability.h"

namespace raid
{

void AbilityComponent::AddAbility(const AbilityDefinition& def)
{
	mainAssert(GetAbility(def.Id) == nullptr);
	m_Abilities.push_back(new Ability(def));
}

Ability* AbilityComponent::GetAbility(AbilityId ability)
{
	for (Ability* a : m_Abilities)
	{
		if (a->GetId() == ability)
		{
			return a;
		}
	}

	return nullptr;
}

} // namespace raid
