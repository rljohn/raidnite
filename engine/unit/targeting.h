#pragma once

#include "engine/entity/component.h"

namespace raid
{

enum class TargetFilter
{
	Invalid,
	Friendly,
	Enemy,
	Any
};

enum class TargetBehaviour
{
	Random,
	Closest,
	Furthest,
	Next
};

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
