#pragma once

#include "engine/types.h"
#include "engine/game/game.h"
#include "engine/entity/entity.h"

namespace raid
{

class Entity;
class Component
{
public:

    Component() = delete;
    Component(Entity& parent)
        : m_Parent(parent)
    {
    }

    virtual ~Component() = default;

    virtual void Init() {}
    virtual void Update(GameFrame& /* frame */) {}

protected:

    Entity& m_Parent;
};


} // namespace raid