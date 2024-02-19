#pragma once

#include "log_channels.h"

// Engine Includes
#include "engine/types.h"
#include "engine/system/stringutil.h"

// Third Party Includes"
#include "thirdparty/spdlog/spdlog.h"
#include "thirdparty/spdlog/sinks/basic_file_sink.h"

namespace raid
{

class Logger
{
public:

	~Logger();

	bool Init(const char* name, const std::wstring& path);
	void Shutdown();

	template<typename... Args>
	bool LogMessage(const LogChannel& channel, LogSeverity severity, const char* format, Args&&... args)
	{
		if (!m_SpdLog)
			return false;

		if (channel.Severity < severity)
			return false;

		switch(severity)
		{
			case LogSeverity::Fatal:
				m_SpdLog->critical(format, std::forward<Args>(args)...);
				break;
			case LogSeverity::Display:
				m_SpdLog->info(format, std::forward<Args>(args)...);
				break;
			case LogSeverity::Error:
				m_SpdLog->error(format, std::forward<Args>(args)...);
				break;
			case LogSeverity::Assert:
				m_SpdLog->error(format, std::forward<Args>(args)...);
				break;
			case LogSeverity::Warning:
				m_SpdLog->warn(format, std::forward<Args>(args)...);
				break;
			case LogSeverity::Verbose:
				m_SpdLog->debug(format, std::forward<Args>(args)...);
				break;
			case LogSeverity::VeryVerbose:
				m_SpdLog->trace(format, std::forward<Args>(args)...);
				break;
		}

		return true;
	}

private:

	void PrintStartupMessage();

	std::shared_ptr<spdlog::logger> m_SpdLog;
	std::string m_Name;
};

class LogSystem
{
public:

	static Logger* CreateLogger(const char* name, const std::wstring& path);

	static void SetDefaultLogger(Logger* logger);
	static Logger* GetDefaultLogger();

	static void AssertBreakMsg(const char* msg);

	static void ResetTickCount();
	static int64_t GetTickCount();
	static void Tick();
	

private:

	static Logger* sm_DefaultLogger;
	static int64_t sm_TickCount;
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

