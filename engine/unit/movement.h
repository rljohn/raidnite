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

	MovementComponent(Entity& parent, TransformComponent& transform);

	void Init() override;
	void Update(const GameFrame& frame) override;

	void ResetPath();
	void SetPath(const TilePath& path);
	const TilePath& GetPath() { return m_Path; }

private:

	void CalculatePosition();
	double GetSpeed() const;

	TransformComponent& m_Transform;
	IAttribute* m_SpeedAttribute;
	double m_ConstantSpeed;
	TilePath m_Path;
	int m_TilePathIndex;

};

}  // namespace raid
