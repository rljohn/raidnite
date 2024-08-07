#include "engine/pch.h"
#include "engine/unit/states/state.h"
#include "engine/unit/states/state_idle.h"
#include "engine/system/log/logging.h"

#include "thirdparty/spdlog/fmt/bundled/format.h"

namespace raid
{

auto format_as(StateType f) { return fmt::underlying(f); }

const char* StateTypeToString(StateType e)
{
	switch (e)
	{
	case StateType::Invalid: return "Invalid";
	case StateType::Idle: return "Idle";
	case StateType::Moving: return "Moving";
	}

	return "Invalid";
}

StateMachineComponent::StateMachineComponent(Entity& parent)
	: Component(parent)
{
	AddState(new UnitState_Idle());
}

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
	return GetState(t) != nullptr;
}

bool StateMachineComponent::SetState(StateType t)
{
	if (HasState(t))
	{
		m_CurrentState = t;
		return true;
	}

	return false;
}

bool StateMachineComponent::ValidateState(StateType t)
{
	return (t > StateType::Invalid && t < StateType::COUNT);
}

UnitState* StateMachineComponent::GetCurrentState()
{
	return GetState(m_CurrentState);
}

StateType StateMachineComponent::GetCurrentStateType() const
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