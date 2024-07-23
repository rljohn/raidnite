#include "engine/pch.h"
#include "movement.h"

#include "engine/game/game_logic.h"

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
	double speed = GetSpeed();
	if (speed == 0)
	{
		return;
	}

	double tilesPerSecond = GameLogic::GetTilesPerSecond(speed);
	double distance = frame.TimeStepSecs.count() * tilesPerSecond;
}

void MovementComponent::ResetPath()
{
	m_Path.Reset();
}

void MovementComponent::SetPath(const TilePath& path)
{
	m_Path = path;
}

double MovementComponent::GetSpeed() const
{
	if (m_SpeedAttribute)
	{
		return m_SpeedAttribute->GetValue();
	}

	return m_ConstantSpeed;
}

} // namespace raid