#pragma once

#include "engine/entity/component.h"
#include "engine/unit/unit_component.h"

namespace raid
{

enum class StateType
{
	Invalid = -1,
	Idle = 0,
	Moving = 2,
	Ability = 3,

	COUNT
};

const char* StateTypeToString(StateType e);

class StateMachineComponent;

class UnitState
{
public:

	virtual StateType GetType() const = 0;

	virtual void Init(StateMachineComponent& machine) {}
	virtual void OnBegin(StateMachineComponent& machine) {}
	virtual void OnEnd(StateMachineComponent& machine) {}
	virtual void Update(StateMachineComponent& machine, const GameFrame& frame) {}

	virtual bool GetDesiredState(StateType& /* state */) const { return false; }
};

class StateMachineComponent : public UnitComponent
{
public:

	StateMachineComponent(Unit& parent);

	bool AddState(UnitState* state);
	bool HasState(StateType t);
	bool SetState(StateType t);

	UnitState* GetCurrentState();
	StateType GetCurrentStateType() const;

	void Update(const GameFrame& /* frame */) override;

private:

	bool ValidateState(StateType t);
	UnitState* GetState(StateType t);
	
	StateType m_CurrentState;
	UnitState* m_States[(int)StateType::COUNT] = { nullptr };
};

} // namespace raid