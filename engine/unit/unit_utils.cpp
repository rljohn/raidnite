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

const char* UnitStatics::GetName(const Entity & entity)
{
	if (NameComponent* name = entity.GetComponent<NameComponent>())
	{
		return name->GetName().c_str();
	}

	return "";
}

bool UnitStatics::IsEntityInRange(const Unit& source, const Entity& target, Distance distance)
{
	TransformComponent* targetTransform = target.GetComponent<TransformComponent>();
	if (!targetTransform)
	{
		return false;
	}

	const Position& targetPos = targetTransform->GetPosition();

	const TransformComponent& transform = source.GetTransform();
	Distance d = Pathfinding::DistanceBetweenTiles(targetPos, transform.GetPosition());
	if (d > distance)
	{
		return false;
	}

	return true;
}

} // namespace raid