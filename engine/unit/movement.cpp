#include "engine/pch.h"
#include "movement.h"

namespace raid
{

void MovementComponent::Init()
{
	if (AttributesComponent* attrs = m_Parent.GetComponent<AttributesComponent>())
	{
		m_SpeedAttribute = attrs->GetAttribute<AttributeType::Speed>();
	}
}

void MovementComponent::Update(const GameFrame& frame)
{
	float speed = GetSpeed();
	if (speed == 0)
		return;
}

void MovementComponent::ResetPath()
{
	m_Path.Reset();
}

float MovementComponent::GetSpeed() const
{
	if (m_SpeedAttribute)
	{
		return m_SpeedAttribute->GetValue();
	}

	return m_ConstantSpeed;
}

} // namespace raid