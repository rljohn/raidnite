#include "engine/pch.h"
#include "state_ability.h"

#include "engine/engine.h"
#include "engine/game/ability.h"
#include "engine/unit/unit.h"

namespace raid
{

void UnitState_Ability::Init(StateMachineComponent& machine)
{
	m_AbilityComponent = machine.GetUnit().GetComponent<AbilityComponent>();
}

void UnitState_Ability::OnBegin(StateMachineComponent& machine)
{
	Ability* ability = m_AbilityComponent->GetCurrentAbility();
	unitAssert(ability != nullptr);

	m_StartFrame = GetCurrentGameFrame();

	// TODO: Calculate cooldown of spell in frames
}

void UnitState_Ability::OnEnd(StateMachineComponent& machine)
{
	if (m_Completed)
	{

	}
	else
	{
		// TODO: Trigger interupt event
	}
}

void UnitState_Ability::Update(StateMachineComponent& machine)
{
	// TODO: 
}

} // namespace raid