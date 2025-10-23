#include "engine/pch.h"
#include "state_ability.h"

#include "engine/engine.h"
#include "engine/game/ability.h"
#include "engine/game/game_events.h"
#include "engine/unit/unit.h"
#include "engine/unit/targeting.h"

namespace raid
{

void UnitState_Ability::Init()
{
	m_AbilityComponent = GetUnit().GetComponent<AbilityComponent>();
	unitAssert(m_AbilityComponent);

	m_TargetingComponent = GetUnit().GetComponent<TargetingComponent>();
	unitAssert(m_TargetingComponent);
}

void UnitState_Ability::OnBegin()
{
	Ability* ability = m_AbilityComponent->GetCurrentAbility();
	if (!ability)
	{
		unitAssert(ability != nullptr);
		return;
	}

	m_StartFrame = GetCurrentGameFrame();
	m_BaseCooldown = ability->GetBaseCooldown();
	m_Target = m_TargetingComponent->GetTarget();

	Duration duration = Time::ToNanoSeconds(m_BaseCooldown);
	m_BaseCooldownInFrames = Game::GetEngine()->DurationToFrames(duration);

	AbilityBeginEvent evt(&GetUnit(), m_Target);
	Game::DispatchGameEvent(evt);
}

void UnitState_Ability::OnEnd()
{
	AbilityEndEvent evt(&GetUnit(), m_Target, m_Success);
	Game::DispatchGameEvent(evt);
	m_Target = nullptr;
	m_StartFrame = 0;
	m_BaseCooldown = 0;
	m_BaseCooldownInFrames = 0;
}

void UnitState_Ability::Update(const GameFrame& frame)
{
	// TODO: Calculate progress
	m_Finished = false;
}

bool UnitState_Ability::GetDesiredState(StateType& state) const
{
	if (m_Finished)
	{
		state = StateType::Idle;
		return true;
	}
	else
	{
		return false;
	}
}

} // namespace raid