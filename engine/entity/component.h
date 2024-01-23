#pragma once

#include "engine/types.h"
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

protected:

    Entity& m_Parent;
};


} // namespace raid