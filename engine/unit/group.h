#pragma once

#include "engine/system/container.h"

namespace raid
{

class Entity;

template<int _SIZE>
class Group
{
public:

	Group()
	{
		m_List.reserve(_SIZE);
	}

	bool AddUnit(Entity* entity)
	{
		if (entity == nullptr)
		{
			return false;
		}

		if (IsFull())
		{
			return false;
		}

		if (VectorContains(m_List, entity))
		{
			return true;
		}

		m_List.push_back(entity);
		return true;
	}

	void RemoveUnit(Entity* entity)
	{
		VectorRemove(m_List, entity);
	}

	void RemoveUnitAt(int index)
	{
		VectorDelete(m_List, index);
	}

	bool Contains(Entity* entity) const
	{
		return VectorContains(m_List, entity);
	}

	bool IsEmpty() const { return m_List.size() == 0; }
	bool IsFull() const { return m_List.size() == _SIZE; }

private:

	std::vector<Entity*> m_List;
};

} // namespace raid