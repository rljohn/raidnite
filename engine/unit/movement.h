#pragma once

#include "engine/entity/component.h"
#include "engine/entity/transform.h"
#include "engine/unit/attribute.h"
#include "engine/map/pathfinding.h"

namespace raid
{

struct IAttribute;

class MovementComponent : public Component
{
public:

	MovementComponent(Entity& parent, TransformComponent& transform)
		: Component(parent)
		, m_Transform(transform)
		, m_SpeedAttribute(nullptr)
		, m_ConstantSpeed(0.0f)
	{
	}

	void Init() override;
	void Update(const GameFrame& frame) override;

	void ResetPath();
	void SetPath(const TilePath& path);
	const TilePath& GetPath() { return m_Path; }

private:

	float GetSpeed() const;

	TransformComponent& m_Transform;
	IAttribute* m_SpeedAttribute;
	float m_ConstantSpeed;
	TilePath m_Path;

};

}  // namespace raid
