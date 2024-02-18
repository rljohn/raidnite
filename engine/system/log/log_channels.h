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
			__tmp->LogMessage(channel, severity, "Condition Failed: ", #cond);								\
			__tmp->LogMessage(channel, severity, fmt, __VA_ARGS__);											\
		}																									\
	}

// Helper Macros
#define mainFatal(fmt, ...) logMessage(fmt, LogChannels::Main, LogSeverity::Fatal, fmt, __VA_ARGS__)
#define mainDisplay(fmt, ...) logMessage(fmt, LogChannels::Main, LogSeverity::Display, fmt, __VA_ARGS__)
#define mainError(fmt, ...) logMessage(fmt, LogChannels::Main, LogSeverity::Error, fmt, __VA_ARGS__)
#define mainWarning(fmt, ...) logMessage(fmt, LogChannels::Main, LogSeverity::Warning, fmt, __VA_ARGS__)
#define mainAssert(cond, fmt, ...) logAssert(cond, fmt, LogChannels::Main, LogSeverity::Error, fmt, __VA_ARGS__)

} // namespace raid