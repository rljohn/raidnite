#pragma once

#include "unit.h"
#include "engine/entity/entity.h"
#include "engine/unit/attribute.h"
#include "engine/unit/power.h"

namespace raid
{

class UnitStatics
{
public:

	static void InitUnitHealth(Unit& unit, double min, double max);
	static bool GetUnitLevel(const Entity& entity, double& out_level);

	template<AttributeType ATTR>
	static bool GetAttribute(const Entity& entity, double& out_value)
	{
		if (AttributesComponent* attrs = entity.GetComponent<AttributesComponent>())
		{
			if (IAttribute* attr = attrs->GetAttribute<ATTR>())
			{
				out_value = attr->GetValue();
				return true;
			}
		}

		return false;
	}
};

} // namespace raid