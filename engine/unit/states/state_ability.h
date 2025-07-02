#pragma once
#include "state.h"

namespace raid
{

class AbilityComponent;

class UnitState_Ability : public UnitState
{
public:

	StateType GetType() const override  { return StateType::Ability; }

	void Init(StateMachineComponent& machine) override;
	void OnBegin(StateMachineComponent& machine) override;
	void OnEnd(StateMachineComponent& machine) override;
	void Update(StateMachineComponent& machine) override;

private:

	AbilityComponent* m_AbilityComponent = nullptr;
	bool m_Completed = false;
	Frame m_StartFrame = 0;
};

} // namespace raid