#pragma once

#include <unordered_map>
#include <typeindex>

#include "engine/types.h"

namespace raid
{

class Component;
class World;

class Entity
{
public:

    Entity();

    template <typename T, typename... Args>
    T& AddComponent(Args&&... args)
    {
        T* component = new T(*this, std::forward<Args>(args)...);
        m_Components[typeid(T)] = component;
        return *component;
    }

    template <typename T>
    T* GetComponent() const
    {
        auto it = m_Components.find(typeid(T));
        return (it != m_Components.end()) ? static_cast<T*>(it->second) : nullptr;
    }

    void SetId(EntityId id) { m_Id = id; }
    EntityId GetId() const { return m_Id; }

    virtual void Init();

private:

    std::unordered_map<std::type_index, Component*> m_Components;
    EntityId m_Id;
};

class IEntityLocator
{
public:

    virtual Entity* FindEntity(const EntityId id) = 0;
};

} // namespace raid