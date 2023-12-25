#pragma once

#include "engine/system/inlist.h"

namespace raid
{

template <typename T, int _SIZE>
class IntrusivePool
{
	T m_Objects[_SIZE];
	inlist<T> m_Available;
	typedef inlist_node<T> Node;

public:

	void Init(size_t nodeOffset)
	{
		for (int i = 0; i < _SIZE; i++)
		{
			T* value = &m_Objects[i];
			Node* node = reinterpret_cast<Node*>(reinterpret_cast<uintptr_t>(value) + nodeOffset);
			m_Available.push_back(*node);
		}
	}

	int GetNumAvailable() const
	{
		return m_Available.size();
	}

	T* Create()
	{
		Node* node = m_Available.front();
		m_Available.pop_front();
		return node->data;
	}

	void Free(T* value)
	{
		if (value)
		{
			m_Available.push_back(value->m_Node);
		}
	}
};

} // namespace raid