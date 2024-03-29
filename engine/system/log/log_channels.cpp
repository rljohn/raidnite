#include "engine/pch.h"
#include "log_channels.h"

namespace raid
{

constexpr const LogSeverity DefaultLogSeverity = RAID_RELEASE ? LogSeverity::Display : LogSeverity::Verbose;

// Built-In Engine channels
LogChannel LogChannels::Main("Main", DefaultLogSeverity);
LogChannel LogChannels::Unit("Unit", DefaultLogSeverity);
LogChannel LogChannels::Map("Map", DefaultLogSeverity);

} // namespace raid