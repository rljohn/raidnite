#pragma once

#include "component.h"

namespace raid
{

class TransformComponent : public Component
{
public:
    
    TransformComponent(Entity& parent)
        : Component(parent)
    {
    }

    const Position& GetPosition() { return m_Position; }
    void SetPosition(const Position& pos) { m_Position = pos; }

    const Location& GetLocation() { return m_Location; }
    void GetLocation(const Location& pos) { m_Location = pos; }

private:

    Position m_Position;
    Location m_Location;
};

}