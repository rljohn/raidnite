#pragma once
#include "state.h"

namespace raid
{

class UnitState_Idle : public UnitState
{
public:

	UnitState_Idle(StateMachineComponent& machine)
		: UnitState(machine)
	{
	}

	StateType GetType() const override {
		return StateType::Idle;
	}

	bool GetDesiredState(StateType& /* state */) const override;
};

} // namespace raid