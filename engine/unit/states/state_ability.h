#pragma once
#include "state.h"

namespace raid
{

class AbilityComponent;
class TargetingComponent;

class UnitState_Ability : public UnitState
{
public:

	StateType GetType() const override  { return StateType::Ability; }

	void Init(StateMachineComponent& machine) override;
	void OnBegin(StateMachineComponent& machine) override;
	void OnEnd(StateMachineComponent& machine) override;
	void Update(StateMachineComponent& machine, const GameFrame& frame) override;
	bool GetDesiredState(StateType& /* state */) const override;

private:

	AbilityComponent* m_AbilityComponent = nullptr;
	TargetingComponent* m_TargetingComponent = nullptr;
	Entity* m_Target = nullptr;
	bool m_Success = false;
	bool m_Finished = false;
	Frame m_StartFrame = 0;
	double m_BaseCooldown = 0;
	Frame m_BaseCooldownInFrames = 0;
};

} // namespace raid