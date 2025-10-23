#include "engine/pch.h"
#include "state_idle.h"
#include "engine/unit/unit.h"

namespace raid
{

bool UnitState_Idle::GetDesiredState(StateType& state) const
{
	const Unit& unit = GetUnit();
	if (const AIComponent* ai = unit.GetAi())
	{
		if (ai->IsReadyToCast())
		{
			state = StateType::Ability;
			return true;
		}
	}

	return false;
}

} // namespace raid