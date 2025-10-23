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

	UnitState(StateMachineComponent& parent);

	virtual StateType GetType() const = 0;

	virtual void Init() {}
	virtual void OnBegin() {}
	virtual void OnEnd() {}
	virtual void Update(const GameFrame& frame) {}

	virtual bool GetDesiredState(StateType& /* state */) const { return false; }

	Unit& GetUnit();
	const Unit& GetUnit() const;

protected:

	StateMachineComponent& m_Machine;
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