#pragma once

#include "log_channels.h"

// Engine Includes
#include "engine/types.h"
#include "engine/system/stringutil.h"

// Third Party Includes"
#include "thirdparty/spdlog/spdlog.h"
#include "thirdparty/spdlog/sinks/basic_file_sink.h"

#define MSVC_LOGGER (_DEBUG)

namespace raid
{

class Logger
{
public:

	~Logger();

	bool Init(const char* name, const std::wstring& path);
	void Shutdown();

	template<typename... Args>
	bool LogMessage(const LogChannel& channel, LogSeverity severity, fmt::format_string<Args...> format, Args&&... args)
	{
		if (channel.Severity < severity)
			return false;

		bool output = false;
		for (auto& logger : m_Loggers)
		{
			if (logger == nullptr)
				continue;

			output = true;
			switch (severity)
			{
			case LogSeverity::Fatal:
				logger->critical(format, std::forward<Args>(args)...);
				break;
			case LogSeverity::Display:
				logger->info(format, std::forward<Args>(args)...);
				break;
			case LogSeverity::Error:
				logger->error(format, std::forward<Args>(args)...);
				break;
			case LogSeverity::Assert:
				logger->error(format, std::forward<Args>(args)...);
				break;
			case LogSeverity::Warning:
				logger->warn(format, std::forward<Args>(args)...);
				break;
			case LogSeverity::Verbose:
				logger->debug(format, std::forward<Args>(args)...);
				break;
			case LogSeverity::VeryVerbose:
				logger->trace(format, std::forward<Args>(args)...);
				break;
			}
		}

		return output;
	}

private:

	void PrintStartupMessage();
	std::array<std::shared_ptr<spdlog::logger>, 2> m_Loggers;
	std::string m_Name;
};

class LogSystem
{
public:

	static Logger* CreateLogger(const char* name, const std::wstring& path);

	static void SetDefaultLogger(Logger* logger);
	static Logger* GetDefaultLogger();

	static void AssertBreakMsg(const char* msg);
	

private:

	static Logger* sm_DefaultLogger;
};

struct LoggerRAII
{
	LoggerRAII() = delete;
	LoggerRAII(Logger* logger)
		: m_Logger(logger)
	{
		LogSystem::SetDefaultLogger(logger);
	}

	~LoggerRAII()
	{
		if (m_Logger)
		{
			m_Logger->Shutdown();
			delete m_Logger;
		}
		
		LogSystem::SetDefaultLogger(nullptr);
	}

	Logger* m_Logger;
};

} // namespace raid

