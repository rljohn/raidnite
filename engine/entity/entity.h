#pragma once

#include <unordered_map>
#include <typeindex>

#include "engine/types.h"

namespace raid
{

class Component;
class World;
struct GameEvent;
struct GameFrame;

class Entity
{
public:

    Entity();
    ~Entity();

    template <typename T, typename... Args>
    T& AddComponent(Args&&... args)
    {
        static_assert(std::is_base_of<Component, T>::value,
            "AddComponent type must be derived from Component");

        T* component = new T(*this, std::forward<Args>(args)...);
        return AddComponent(component);
    }

    template <typename T>
    T* GetComponent() const
    {
        auto it = m_Components.find(typeid(T));
        return (it != m_Components.end()) ? static_cast<T*>(it->second) : nullptr;
    }

    void SetId(EntityId id) { m_Id = id; }
    EntityId GetId() const { return m_Id; }

    void Init();

    virtual void OnInit() {}
    virtual void Update(const GameFrame& /* frame */);
    virtual void OnGameEvent(const GameEvent& evt);

protected:

    template <typename T>
    T& AddComponent(T* component)
    {
        m_Components[typeid(T)] = component;
        return *component;
    }

private:

    std::unordered_map<std::type_index, Component*> m_Components;
    EntityId m_Id;
};

class IEntityListener
{
public:

    virtual void OnEntityRemoved(const Entity* entity) = 0;
};

class IEntityManager
{
public:

    virtual Entity* FindEntity(const EntityId id) = 0;
    virtual void OnGameEvent(const GameEvent& e) = 0;
    virtual void RegisterEntity(Entity* unit) = 0;
    virtual void UnRegisterEntity(Entity* unit) = 0;
    virtual void Reset() = 0;
    virtual size_t GetEntityCount() const = 0;
    virtual Entity* GetEntity(size_t idx) = 0;
    virtual void AddEntityListener(IEntityListener* listener) = 0;
    virtual void RemoveEntityListener(IEntityListener* listener) = 0;

    using EntityCallback = std::function<bool(Entity*)>;
    void ForEach(const EntityCallback& callback)
    {
        for (int i = 0; i < GetEntityCount(); i++)
        {
            if (callback(GetEntity(i)))
                break;
        }
    }
};

inline EntityId GetEntityId(Entity* e)
{
    return e ? e->GetId() : InvalidEntityId;
}


} // namespace raid