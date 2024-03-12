#pragma once

#include "engine/entity/component.h"
#include "thirdparty/spdlog/fmt/bundled/format.h"

namespace raid
{

enum class StateType
{
	Invalid = -1,
	Idle = 0,
	Moving = 2,

	COUNT
};
auto format_as(StateType f) { return fmt::underlying(f); }

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

	bool AddState(UnitState* state);
	bool HasState(StateType t);

	UnitState* GetCurrentState();
	StateType GetCurrentStateType();

	void Update();

private:

	bool ValidateState(StateType t);
	UnitState* GetState(StateType t);
	
	StateType m_CurrentState;
	UnitState* m_States[(int)StateType::COUNT] = { nullptr };
};

} // namespace raid