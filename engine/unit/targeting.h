#pragma once

#include "engine/entity/component.h"

namespace raid
{

class TargetingComponent : public Component
{
public:

	TargetingComponent(Entity& parent)
		: Component(parent)
		, m_Target(nullptr)
	{
	}

	bool HasTarget() const;
	Entity* GetTarget() const;

private:

	Entity* m_Target;
};

} // namespace raid
