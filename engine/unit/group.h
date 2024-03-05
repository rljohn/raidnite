#pragma once

#include "engine/system/container.h"

namespace raid
{

class Entity;

class Group
{
public:

    Group();
    Group(int size);

    void Init(int size);
    bool AddUnit(Entity* entity);
    void RemoveUnit(Entity* entity);
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