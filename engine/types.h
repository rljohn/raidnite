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
#include <array>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <set>

#include "engine/system/math/vector2.h"

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

	// Map
	using Position = IntVector2D;

	


} // namespace raid


