#pragma once

#include "engine/system/inlist.h"

namespace raid
{

template<typename T>
class IPool
{
public:

	virtual int GetNumAvailable() const = 0;
	virtual T* Create() = 0;
	virtual void Free(T* value) = 0;
};

template <typename T, int _SIZE>
class IntrusivePool : public IPool<T>
{
	T m_Objects[_SIZE];
	inlist<T> m_Available;
	typedef inlist_node<T> Node;

public:
	
	IntrusivePool()
	{
		size_t offset = offsetof(T, m_Node);
		Init(offset);
	}

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

	T* Create() override
	{
		Node* node = m_Available.front();
		m_Available.pop_front();
		return node->data;
	}

	void Free(T* value) override
	{
		if (value)
		{
			m_Available.push_back(value->m_Node);
		}
	}
};

} // namespace raid