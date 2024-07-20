#pragma once
#include "state.h"

namespace raid
{

class UnitState_Idle : public UnitState
{
public:

	StateType GetType() const override {
		return StateType::Idle;
	}

};

} // namespace raid