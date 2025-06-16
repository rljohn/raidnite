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
	static const char* GetName(const Entity& entity);

	template<AttributeType ATTR, typename VALUE_TYPE>
	static bool GetAttribute(const Entity& entity, VALUE_TYPE& out_value)
	{
		if (AttributesComponent* attrs = entity.GetComponent<AttributesComponent>())
		{
			if (IAttribute* attr = attrs->GetAttribute<ATTR>())
			{
				out_value = (VALUE_TYPE)attr->GetValue();
				return true;
			}
		}

		return false;
	}

	static bool IsEntityInRange(const Unit& source, const Entity& target, Distance distance);
};

} // namespace raid