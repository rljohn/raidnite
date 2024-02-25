#include "engine/pch.h"
#include "engine/unit/states/state.h"
#include "engine/system/log/logging.h"

namespace raid
{

bool StateMachineComponent::AddState(UnitState* state)
{
	if (!state)
	{
		unitError("Invalid state: nullptr");
		return false;
	}

	StateType t = state->GetType();
	if (HasState(t))
	{
		unitError("Already has state: {}", t);
		return false;
	}

	m_States[(int)t] = state;
	return true;
}

bool StateMachineComponent::HasState(StateType t)
{
	return GetState(GetCurrentStateType());
}

bool StateMachineComponent::ValidateState(StateType t)
{
	return (t > StateType::Invalid && t < StateType::COUNT);
}

UnitState* StateMachineComponent::GetCurrentState()
{
	return GetState(m_CurrentState);
}

StateType StateMachineComponent::GetCurrentStateType()
{
	return m_CurrentState;
}

UnitState* StateMachineComponent::GetState(StateType t)
{
	if (!ValidateState(t))
	{
		unitError("Invalid state: {}", t);
		return nullptr;
	}
	
	return m_States[(int)t];
}

void StateMachineComponent::Update()
{
	UnitState* state = GetCurrentState();
	if (!state)
		return;

	state->Update();

	StateType desiredState;
	if (state->GetDesiredState(desiredState))
	{
		UnitState* nextState = GetState(desiredState);
		if (nextState)
		{
			state->OnEnd();
			m_CurrentState = desiredState;
			nextState->OnBegin();
		}
		else
		{
			unitError("Unable to transition to state: {}, not found.", desiredState);
		}
	}
}

} // namespace raid