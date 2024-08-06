#include "engine/pch.h"
#include "movement.h"

#include "engine/game/game_logic.h"
#include "engine/map/tile.h"
#include "engine/system/math/vector_math.h"

namespace raid
{
	
MovementComponent::MovementComponent(Entity& parent, TransformComponent& transform)
	: Component(parent)
	, m_Transform(transform)
	, m_SpeedAttribute(nullptr)
	, m_ConstantSpeed(0.0f)
	, m_TilePathIndex(0)
{
}

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

	const Location& location = m_Transform.GetLocation();

	Location next = VectorMath::FromPosition(m_Path[m_TilePathIndex]->GetPosition());
	
	// For now: 2D motion, maintain Z
	next.SetZ(location.GetZ());

	Location newLoc = VectorMath::TranslateTo(location, next, distance);
	m_Transform.SetLocation(newLoc);
	CalculatePosition();
}

void MovementComponent::CalculatePosition()
{
	const Location& location = m_Transform.GetLocation();

	Position newPos((int)std::round(location.GetX()), (int)std::round(location.GetY()));
	if (newPos != m_Transform.GetPosition())
	{
		m_Transform.SetPosition(newPos);
	}
}

void MovementComponent::ResetPath()
{
	m_Path.Reset();
	m_TilePathIndex = 0;
}

void MovementComponent::SetPath(const TilePath& path)
{
	m_Path = path;
	m_TilePathIndex = 0;
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