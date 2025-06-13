#pragma once

#include <vector>
#include <set>
#include <map>
#include <array>
#include <unordered_map>
#include <unordered_set>

namespace raid
{
	// Map Find or Null
	template<typename T, typename U>
	U GetMapEntry(std::unordered_map<T, U>& map, const T& key)
	{
		auto it = map.find(key);
		if (it != map.end())
		{
			return it->second;
		}

		return nullptr;
	}

	// Vector removal
	template <typename T>
	bool VectorRemove(std::vector<T>& v, const T& value)
	{
		auto it = std::find(v.begin(), v.end(), value);

		if (it != v.end())
		{
			v.erase(it);
			return true;
		}

		return false;
	}

	template <typename T>
	bool VectorRemoveConst(std::vector<T*>& v, const T* value)
	{
		return VectorRemove(v, const_cast<T*>(value));
	}

	template <typename T>
	bool VectorContains(const std::vector<T>& v, const T& value)
	{
		return std::find(v.begin(), v.end(), value) != v.end();
	}

	template<typename T>
	void VectorDelete(std::vector<T>& vec, std::size_t index) 
	{
		if (index >= vec.size()) 
		{
			return;
		}

		vec.erase(vec.begin() + index);
	}

	template<typename T>
	T* VectorGet(std::vector<T>& vec, int index)
	{
		if (index >= 0 && index < vec.size())
		{
			return vec[index];
		}
		else
		{
			return nullptr;
		}
	}

	// Declare functions required for Range-Based for loop
#define DECLARE_ITERABLE(Container) public: \
    using iterator = decltype(Container)::iterator;                      \
    using const_iterator = decltype(Container)::const_iterator;          \
    iterator begin() { return Container.begin(); }                       \
    iterator end() { return Container.end(); }                           \
    const_iterator begin() const { return Container.begin(); }        \
    const_iterator end() const { return Container.end(); }

	template<typename T>
	bool SetContains(const std::set<T>& s, const T& value)
	{
		return s.find(value) != s.end();
	}

	template<typename T, int _SIZE>
	bool ArrayContains(const std::array<T, _SIZE>& s, const T& value)
	{
		return s.find(value) != s.end();
	}

	template<typename T>
	T* ListGet(std::list<T*>& list, int index)
	{
		if (index >= 0 && index < list.size())
		{
			auto it = list.begin();
			std::advance(it, index);
			return *it;
		}
		else
		{
			return nullptr;
		}
	}

} // namespace raid