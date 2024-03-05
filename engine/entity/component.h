#pragma once

#include "engine/types.h"
#include "engine/game/game.h"
#include "engine/entity/entity.h"

namespace raid
{

class Entity;
struct GameEvent;

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
    virtual void Update(const GameFrame& /* frame */) {}
    virtual void OnGameEvent(const GameEvent& evt) {}
    virtual void Shutdown() {}

protected:

    Entity& m_Parent;
};


} // namespace raid