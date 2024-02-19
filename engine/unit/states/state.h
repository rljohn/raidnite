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
	virtual void OnEndState() {}
};

class StateMachineComponent : public Component
{
public:

	bool AddState(UnitState* state);
	bool HasState(StateType t);

private:

	bool ValidateState(StateType t);

	UnitState* m_States[(int)StateType::COUNT] = { nullptr };
};

} // namespace raid