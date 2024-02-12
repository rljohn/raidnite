#pragma once

// Intrusive List
// Primarily used for containers of large pools of objects, to avoid
// dynamic allocations when objects are added/removed from the pool and
// assigned to various objects.

namespace raid
{

template<typename T>
struct inlist_node
{
	T* data;
	inlist_node* next;
	inlist_node* previous;

	inlist_node(T* d)
		: data(d)
		, next(nullptr)
		, previous(nullptr)
	{
	}

private:

	inlist_node() = delete;
};

template<typename T>
class inlist
{
public:

	typedef inlist_node<T> Node;
	Node* m_Head = nullptr;
	Node* m_Tail = nullptr;
	int m_Count = 0;

	void push_back(Node& node)
	{
		if (m_Tail)
		{
			m_Tail->next = &node;

			node.previous = m_Tail;
			node.next = nullptr;

			m_Tail = &node;
		}
		else
		{
			m_Head = &node;

			node.previous = nullptr;
			node.next = nullptr;

			m_Tail = &node;
		}

		m_Count++;
	}

	void push_front(Node& node)
	{
		if (m_Head)
		{
			m_Head->previous = &node;

			node.previous = nullptr;
			node.next = m_Head;

			m_Head = &node;
		}
		else
		{
			m_Head = &node;

			node.previous = nullptr;
			node.next = nullptr;

			m_Tail = &node;
		}

		m_Count++;
	}

	void remove(Node& node)
	{
		Node* previous = node.previous;
		Node* next = node.next;

		previous->next = &next;
		next->previous = &previous;

		node->next = nullptr;
		node->previous = nullptr;

		m_Count--;
	}

	Node* front()
	{
		return m_Head;
	}

	void pop_front()
	{
		if (m_Head)
		{
			// clear tail if list size is 1
			if (m_Tail == m_Head)
			{
				m_Tail = nullptr;
			}

			Node* next = m_Head->next;
			if (next)
			{
				next->previous = nullptr;
			}
			m_Head = next;

			m_Count--;
		}
	}

	Node* back()
	{
		return m_Tail;
	}

	void pop_back()
	{
		if (m_Tail)
		{
			if (m_Head == m_Tail)
			{
				m_Head == nullptr;
			}

			Node* previous = m_Tail->previous;
			if (previous)
			{
				previous->next = nullptr;
			}

			m_Tail = previous;
			m_Count--;
		}
	}

	class iterator {
	public:
		explicit iterator(Node* node) : current(node) {}

		T& operator*() const {
			return current->data;
		}

		iterator& operator++() {
			current = current->next;
			return *this;
		}

		bool operator!=(const iterator& other) const {
			return current != other.current;
		}

	private:
		Node* current;
	};

	iterator begin() {
		return iterator(m_Head);
	}

	iterator end() {
		return iterator(nullptr);
	}

	class const_iterator 
	{
	public:
		explicit const_iterator(const Node* node) 
			: current(node) 
		{
		}

		const T& operator*() const
		{
			return *current->data;
		}

		const_iterator& operator++()
		{
			current = current->next;
			return *this;
		}

		bool operator!=(const const_iterator& other) const 
		{
			return current != other.current;
		}

	private:
		const Node* current;
	};

	const_iterator begin() const {
		return const_iterator(m_Head);
	}

	const_iterator end() const {
		return const_iterator(nullptr);
	}

	int size() const
	{
		return m_Count;
	}

	bool empty() const
	{
		return m_Count == 0;
	}
};

} // namespace raid