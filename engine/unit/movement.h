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
	void SetPath(const TilePath& path, bool allowSkip = true);
	const TilePath& GetPath() { return m_Path; }

	int GetTilePathIndex() const { return m_TilePathIndex; }

	void Warp(const Position& pos);

private:

	void CalculatePosition();
	void IncrementTilePathIndex();
	double GetSpeed() const;
	Tile* GetNextTile();

	TransformComponent& m_Transform;
	IAttribute* m_SpeedAttribute;
	double m_ConstantSpeed;
	TilePath m_Path;
	int m_TilePathIndex;

};

}  // namespace raid
