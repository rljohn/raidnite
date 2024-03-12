#pragma once

#include "engine/entity/component.h"

namespace raid
{

class MovementComponent : public Component
{
public:

	MovementComponent(Entity& parent)
		: Component(parent)
	{
	}

private:

	Position m_TargetPosition;
};

}  // namespace raid
