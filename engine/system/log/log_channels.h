#pragma once

#include "engine/types.h"

namespace raid
{

enum class LogSeverity
{
	Fatal,
	Display,
	Error,
	Assert,
	Warning,
	Verbose,
	VeryVerbose,
};

struct LogChannel
{
	LogChannel() = delete;
	LogChannel(const char* name, const LogSeverity severity)
		: Name(name)
		, Severity(severity)
	{
	}

	std::string Name;
	LogSeverity Severity;
};

// Built-In channels or engine
namespace LogChannels
{
	extern LogChannel Main;
	extern LogChannel Unit;
	extern LogChannel Map;
};

#define CONCAT(a, b) a ## b
#define CONCAT3(a, b, c) a ## b ## c

#define logMessage(fmt, channel, severity, ...)														\
		if (raid::Logger* __tmp = raid::LogSystem::GetDefaultLogger()) {											\
			__tmp->LogMessage(channel, severity, CONCAT("[{}] ",fmt), channel.Name, __VA_ARGS__);		\
		}

	#define logAssertMessage(cond, fmt, channel, severity, ...)														\
		if (Logger* __tmp = raid::LogSystem::GetDefaultLogger()) {													\
			if (!(cond)) {																						\
				raid::LogSystem::AssertBreakMsg(CONCAT3("Condition Failed: ", #cond, "\n\nDo you want to debug?"));	\
				__tmp->LogMessage(channel, severity, "Condition Failed: {}", #cond);								\
				__tmp->LogMessage(channel, severity, fmt, __VA_ARGS__);											\
			}																									\
		}

#if RAID_LOGGING
	#define logFatal logMessage
	#define logDisplay logMessage
	#define logError logMessage
	#define logWarning logMessage
	#define logAssert logAssertMessage
	#define logVerbose logMessage
#else
	#define logFatal logMessage
	#define logDisplay logMessage
	#define logError logMessage
	#define logWarning(...)
	#define logAssert(...)
	#define logVerbose(...)
#endif

// Main Channel
#define mainFatal(fmt, ...) logFatal(fmt, raid::LogChannels::Main, raid::LogSeverity::Fatal, __VA_ARGS__)
#define mainDisplay(fmt, ...) logDisplay(fmt, raid::LogChannels::Main, raid::LogSeverity::Display, __VA_ARGS__)
#define mainError(fmt, ...) logError(fmt, raid::LogChannels::Main, raid::LogSeverity::Error, __VA_ARGS__)
#define mainWarning(fmt, ...) logWarning(fmt, raid::LogChannels::Main, raid::LogSeverity::Warning, __VA_ARGS__)
#define mainAssert(cond) logAssert(cond, "{}", raid::LogChannels::Main, raid::LogSeverity::Error, #cond)
#define mainAssertf(cond, fmt, ...) logAssert(cond, fmt, raid::LogChannels::Main, raid::LogSeverity::Error, #cond)
#define mainVerbose(fmt, ...) logVerbose(fmt, raid::LogChannels::Main, raid::LogSeverity::Verbose, __VA_ARGS__)

// Unit Channel
#define unitFatal(fmt, ...) logFatal(fmt, LogChannels::Unit, raid::LogSeverity::Fatal, __VA_ARGS__)
#define unitDisplay(fmt, ...) logDisplay(fmt, LogChannels::Unit, raid::LogSeverity::Display, __VA_ARGS__)
#define unitError(fmt, ...) logError(fmt, LogChannels::Unit, raid::LogSeverity::Error, __VA_ARGS__)
#define unitWarning(fmt, ...) logWarning(fmt, LogChannels::Unit, raid::LogSeverity::Warning, __VA_ARGS__)
#define unitAssert(cond) logAssert(cond, "{}", LogChannels::Unit, raid::LogSeverity::Error, #cond)
#define unitAssertf(cond, fmt, ...) logAssert(cond, fmt, LogChannels::Unit, raid::LogSeverity::Error, #cond)
#define unitVerbose(fmt, ...) logVerbose(fmt, LogChannels::Unit, raid::LogSeverity::Verbose, __VA_ARGS__)

// Map Channel
#define mapFatal(fmt, ...) logFatal(fmt, LogChannels::Map, raid::LogSeverity::Fatal, __VA_ARGS__)
#define mapDisplay(fmt, ...) logDisplay(fmt, LogChannels::Map, raid::LogSeverity::Display, __VA_ARGS__)
#define mapError(fmt, ...) logError(fmt, LogChannels::Map, raid::LogSeverity::Error, __VA_ARGS__)
#define mapWarning(fmt, ...) logWarning(fmt, LogChannels::Map, raid::LogSeverity::Warning, __VA_ARGS__)
#define mapAssert(cond) logAssert(cond, "{}", LogChannels::Map, raid::LogSeverity::Error, #cond)
#define mapAssertf(cond, fmt, ...) logAssert(cond, fmt, LogChannels::Map, raid::LogSeverity::Error, #cond)
#define mapVerbose(fmt, ...) logVerbose(fmt, LogChannels::Map, raid::LogSeverity::Verbose, __VA_ARGS__)

} // namespace raid