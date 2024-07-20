#pragma once

#include "engine/entity/component.h"

namespace raid
{

enum class StateType
{
	Invalid = -1,
	Idle = 0,
	Moving = 2,

	COUNT
};

const char* StateTypeToString(StateType e);

class UnitState
{
public:

	virtual StateType GetType() const = 0;

	virtual void OnBegin() {}
	virtual void OnEnd() {}
	virtual void Update() {}

	virtual bool GetDesiredState(StateType& /* state */) { return false; }
};

class StateMachineComponent : public Component
{
public:

	StateMachineComponent(Entity& parent);

	bool AddState(UnitState* state);
	bool HasState(StateType t);
	bool SetState(StateType t);

	UnitState* GetCurrentState();
	StateType GetCurrentStateType() const;

	void Update();

private:

	bool ValidateState(StateType t);
	UnitState* GetState(StateType t);
	
	StateType m_CurrentState;
	UnitState* m_States[(int)StateType::COUNT] = { nullptr };
};

} // namespace raid