#pragma once

#include <cstdint>
#include <chrono>

namespace raid
{

// Time
typedef std::chrono::time_point<std::chrono::system_clock> TimeStamp;
typedef std::chrono::duration<std::chrono::system_clock::rep, std::chrono::system_clock::period> Duration;
typedef std::chrono::milliseconds Milliseconds;
typedef std::chrono::seconds Seconds;

// Spells
typedef int64_t SpellId;
typedef int64_t PlayerId;
#define InvalidPlayerId PlayerId(0)

// Combat Log
typedef int64_t EncounterId;

} // namespace raid


