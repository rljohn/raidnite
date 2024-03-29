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
	if (Logger* __tmp = LogSystem::GetDefaultLogger()) {											\
		__tmp->LogMessage(channel, severity, CONCAT("[{}] ",fmt), channel.Name, __VA_ARGS__);		\
	}

#define logAssert(cond, fmt, channel, severity, ...)														\
	if (Logger* __tmp = LogSystem::GetDefaultLogger()) {													\
		if (!cond) {																						\
			LogSystem::AssertBreakMsg(CONCAT3("Condition Failed: ", #cond, "\n\nDo you want to debug?"));	\
			__tmp->LogMessage(channel, severity, "Condition Failed: {}", #cond);								\
			__tmp->LogMessage(channel, severity, fmt, __VA_ARGS__);											\
		}																									\
	}

// Main Channel
#define mainFatal(fmt, ...) logMessage(fmt, LogChannels::Main, LogSeverity::Fatal, __VA_ARGS__)
#define mainDisplay(fmt, ...) logMessage(fmt, LogChannels::Main, LogSeverity::Display, __VA_ARGS__)
#define mainError(fmt, ...) logMessage(fmt, LogChannels::Main, LogSeverity::Error, __VA_ARGS__)
#define mainWarning(fmt, ...) logMessage(fmt, LogChannels::Main, LogSeverity::Warning, __VA_ARGS__)
#define mainAssert(cond, fmt, ...) logAssert(cond, fmt, LogChannels::Main, LogSeverity::Error, __VA_ARGS__)
#define mainVerbose(fmt, ...) logMessage(fmt, LogChannels::Main, LogSeverity::Verbose, __VA_ARGS__)

// Unit Channel
#define unitFatal(fmt, ...) logMessage(fmt, LogChannels::Unit, LogSeverity::Fatal, __VA_ARGS__)
#define unitDisplay(fmt, ...) logMessage(fmt, LogChannels::Unit, LogSeverity::Display, __VA_ARGS__)
#define unitError(fmt, ...) logMessage(fmt, LogChannels::Unit, LogSeverity::Error, __VA_ARGS__)
#define unitWarning(fmt, ...) logMessage(fmt, LogChannels::Unit, LogSeverity::Warning, __VA_ARGS__)
#define unitAssert(cond, fmt, ...) logAssert(cond, fmt, LogChannels::Unit, LogSeverity::Error, __VA_ARGS__)
#define unitVerbose(fmt, ...) logMessage(fmt, LogChannels::Unit, LogSeverity::Verbose, __VA_ARGS__)

// Map Channel
#define mapFatal(fmt, ...) logMessage(fmt, LogChannels::Map, LogSeverity::Fatal, __VA_ARGS__)
#define mapDisplay(fmt, ...) logMessage(fmt, LogChannels::Map, LogSeverity::Display, __VA_ARGS__)
#define mapError(fmt, ...) logMessage(fmt, LogChannels::Map, LogSeverity::Error, __VA_ARGS__)
#define mapWarning(fmt, ...) logMessage(fmt, LogChannels::Map, LogSeverity::Warning, __VA_ARGS__)
#define mapAssert(cond, fmt, ...) logAssert(cond, fmt, LogChannels::Map, LogSeverity::Error, __VA_ARGS__)
#define mapVerbose(fmt, ...) logMessage(fmt, LogChannels::Map, LogSeverity::Verbose, __VA_ARGS__)

} // namespace raid