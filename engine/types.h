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

#define RAID_LOGGING (RAID_DEBUG)

#define WIDESTR(x) L ## #x

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
#include "engine/system/math/vector3.h"

namespace raid
{
	// Time
	using Nanoseconds = std::chrono::nanoseconds;
	using Microseconds = std::chrono::microseconds;
	using Milliseconds = std::chrono::milliseconds;
	using Seconds = std::chrono::seconds;
	using Minutes = std::chrono::minutes;
	using TimeStamp = std::chrono::time_point<std::chrono::steady_clock>;
	using Duration = Nanoseconds;
	using TimeStep = Nanoseconds;
	using TimeStepSeconds = std::chrono::duration<double>;
	using Frame = uint64_t;

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
	#define HashString(key) StringHash{}(key)

	// Map
	using Position = PositionVector2D;
	static const Position InvalidPosition = Position(-1, -1);
	using Location = Vector3D;
	using Rotation = Vector3D;
	using Distance = double;

	// Factions
	using FactionId = int8_t;
	#define InvalidFactionId FactionId(-1)

	// Localization
	using LocalizationKey = size_t;
	#define InvalidLocalizationKey LocalizationKey(0)

	// Array helper
	#define COUNTOF(arr) (sizeof(arr) / sizeof(arr[0]))

} // namespace raid


