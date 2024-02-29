#include "engine/pch.h"
#include "group.h"

#include "engine/system/log/logging.h"

namespace raid 
{

Group::Group()
{
}

Group::Group(int size)
{
    Init(size);
}

void Group::Init(int size)
{
    unitAssert(IsEmpty(), "Group::Init - Group must be empty");
    m_List.clear();
    m_List.reserve(size);
}

bool Group::AddUnit(Entity* entity)
{
    if (entity == nullptr)
    {
        unitError("Group::AddUnit - Entity nullptr");
        return false;
    }

    if (IsFull())
    {
        unitError("AddUnit - Group full");
        return false;
    }

    if (VectorContains(m_List, entity))
    {
        unitWarning("AddUnit - Already contains entity");
        return true;
    }

    m_List.push_back(entity);
    return true;
}

void Group::RemoveUnit(Entity* entity)
{
    VectorRemove(m_List, entity);
}

void Group::RemoveUnitAt(int index)
{
    VectorDelete(m_List, index);
}

bool Group::Contains(Entity* entity) const
{
    return VectorContains(m_List, entity);
}

bool Group::IsEmpty() const
{
    return m_List.empty();
}

bool Group::IsFull() const
{
    return m_List.size() == m_List.capacity();
}

bool Group::IsValid() const
{
    return m_List.capacity() > 0;
}

size_t Group::GetSize() const
{
    return m_List.size();
}

size_t Group::GetCapacity() const
{
    return m_List.capacity();
}

Entity* Group::GetEntity(const size_t index)
{
    if (index < GetSize())
    {
        return m_List[index];
    }

    return nullptr;
}

} // namespace raid