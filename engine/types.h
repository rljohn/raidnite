#pragma once

#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING

#if NDEBUG
#define RAID_RELEASE (1)
#define RAID_DEBUG (0)
#define RELEASE_ONLY(...) __VA_ARGS__
#define DEBUG_ONLY(...)
#else
#define RAID_RELEASE (0)
#define RAID_DEBUG (1)
#define RELEASE_ONLY(...)
#define DEBUG_ONLY(...) __VA_ARGS__
#endif

#include <algorithm>
#include <cstdint>
#include <chrono>
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <memory>
#include <unordered_map>

namespace raid
{

	// Time
	using TimeStamp = std::chrono::time_point<std::chrono::system_clock>;
	using Duration = std::chrono::duration<std::chrono::system_clock::rep, std::chrono::system_clock::period> ;
	using Milliseconds = std::chrono::milliseconds;
	using Seconds = std::chrono::seconds;

	// Abilities
	using SpellId = int64_t;
	#define InvalidSpellId SpellId(0)

	// Players
	using PlayerId = int64_t;
	#define InvalidPlayerId PlayerId(0)

	// Entities
	using EntityId = int64_t;
	#define InvalidEntityId EntityId(0)	

	// Zone
	using ZoneId = int64_t;
	#define InvalidZoneId ZoneId(0)

	// Combat Log
	using EncounterId = int64_t;

	// Hash
	using StringHash = std::hash<std::string>;

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
	bool RemoveFromVector(std::vector<T>& v, const T& value) 
	{
		auto it = std::find(v.begin(), v.end(), value);

		if (it != v.end()) 
		{
			v.erase(it);
			return true;
		}

		return false;
	}


} // namespace raid


