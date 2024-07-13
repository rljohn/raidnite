#pragma once

#include "engine/system/container.h"
#include "engine/entity/entity.h"

namespace raid
{

class Entity;

class Group : public IEntityListener
{
public:

    Group();
    Group(int size);

    void Init(int size);
    void Shutdown();

    // IEntityListener
    void OnEntityRemoved(const Entity* entity);

    // Public Functinos
    bool AddUnit(Entity* entity);
    void RemoveUnit(const Entity* entity);
    void RemoveUnitAt(size_t index);
    bool Contains(Entity* entity) const;
    bool IsEmpty() const;
    bool IsFull() const;
    bool IsValid() const;
    size_t GetSize() const;
    size_t GetCapacity() const;
    Entity* GetEntity(const size_t index);

private:
    std::vector<Entity*> m_List;
};

} // namespace raid