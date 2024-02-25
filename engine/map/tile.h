#pragma once

#include "engine/entity/entity.h"
#include "engine/system/math/vector2.h"

namespace raid
{

class Tile
{
public:

	const Position& GetPosition() { return m_Position; }
	void SetPosition(const Position& pos) { m_Position = pos; }

	bool IsOccupied() const;

private:

	Position m_Position;
	Entity* m_Occupant;
};

} // namespace raid