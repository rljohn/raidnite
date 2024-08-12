#pragma once

#include "component.h"

namespace raid
{

class TransformComponent : public Component
{
public:
    
    TransformComponent(Entity& parent)
        : Component(parent)
        , m_OccupyingTilePos(InvalidPosition)
    {
    }

    const Position& GetPosition() { return m_Position; }
    void SetPosition(const Position& pos) { m_Position = pos; }

    const Location& GetLocation() { return m_Location; }
    void SetLocation(const Location& pos) { m_Location = pos; }

    const Position& GetOccupyingTile() { return m_OccupyingTilePos; }
    void SetOccupyingTile(const Position& pos) { m_OccupyingTilePos = pos; }

private:

    // Position in 2D space
    Position m_Position;

    // Location in 3D space
    Location m_Location;

    // Position of tile the entity is occupying.
    // Note: While movement is in progress, there is no guarantee the unit overlaps this tile.
    Position m_OccupyingTilePos;
};

}