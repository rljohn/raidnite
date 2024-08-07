#include "engine/pch.h"
#include "unit_utils.h"

namespace raid
{

void UnitStatics::InitUnitHealth(Unit& unit, double min, double max)
{
	IPower* health = unit.GetPower<PowerType::Health>();
	if (health)
	{
		health->SetBaseValues(min, max);
	}
}

bool UnitStatics::GetUnitLevel(const Entity& entity, double& out_level)
{
	if (AttributesComponent* attrs = entity.GetComponent<AttributesComponent>())
	{
		if (IAttribute* attr = attrs->GetAttribute<AttributeType::Level>())
		{
			out_level = attr->GetValue();
			return true;
		}
	}

	return false;
}

} // namespace raid