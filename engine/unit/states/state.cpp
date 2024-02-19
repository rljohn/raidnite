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
	if (!ValidateState(t))
	{
		unitError("Invalid state: {}", t);
		return false;
	}

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
	if (!ValidateState(t))
	{
		unitError("Invalid state: {}", t);
		return false;
	}

	return m_States[(int)t] != nullptr;
}

bool StateMachineComponent::ValidateState(StateType t)
{
	return (t > StateType::Invalid && t < StateType::COUNT);
}

} // namespace raid