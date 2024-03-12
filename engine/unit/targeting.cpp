#include "engine/pch.h"
#include "targeting.h"

namespace raid
{

bool TargetingComponent::HasTarget() const
{
	return m_Target != nullptr;
}

Entity* TargetingComponent::GetTarget() const
{
	return m_Target;
}

}