#include "engine/pch.h"
#include "engine/system/log/logging.h"
#include "engine/game/game.h"

#include "spdlog/pattern_formatter.h"

namespace raid
{

class LogFormatter : public spdlog::custom_flag_formatter
{
public:

	void format(const spdlog::details::log_msg&, const std::tm&, spdlog::memory_buf_t& dest) override
	{
		std::string str = std::to_string(LogSystem::GetTickCount());
		dest.append(std::string(10 - str.length(), '0') + str);
	}

	std::unique_ptr<custom_flag_formatter> clone() const override
	{
		return spdlog::details::make_unique<LogFormatter>();
	}
};

Logger::~Logger()
{
	Shutdown();
}

bool Logger::Init(const char* name, const std::wstring& path)
{
	try
	{
		m_Name = name;

		spdlog::filename_t filename = stringutil::WideStringToUtf8(path);
		m_SpdLog = spdlog::basic_logger_mt(m_Name, filename);

		PrintStartupMessage();

		auto formatter = std::make_unique<spdlog::pattern_formatter>();
		formatter->add_flag<LogFormatter>('*').set_pattern("[%*] [%r] [%l] %v");
		spdlog::set_formatter(std::move(formatter));

		return true;
	}
	catch (const spdlog::spdlog_ex ex)
	{
		std::cout << "Log initialization failed: " << ex.what() << std::endl;
		return false;
	}
}

void Logger::PrintStartupMessage()
{
	SYSTEMTIME st;
	::GetLocalTime(&st);

	const char* suffix = (st.wHour < 12) ? "AM" : "PM";

	char buffer[32];
	sprintf_s(buffer, "%04d/%02d/%02d %02d:%02d:%02d %s", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, suffix);
	m_SpdLog->info("Log Started: {}", std::string(buffer));
}

void Logger::Shutdown()
{
	if (m_SpdLog)
	{
		spdlog::drop(m_Name);
	}

	m_SpdLog = nullptr;
}


Logger* LogSystem::sm_DefaultLogger = nullptr;
int64_t LogSystem::sm_TickCount = 0;

void LogSystem::SetDefaultLogger(Logger* logger)
{
	sm_DefaultLogger = logger;
}

Logger* LogSystem::GetDefaultLogger()
{
	return sm_DefaultLogger;
}

void LogSystem::AssertBreakMsg(const char* DEBUG_ONLY(message))
{
#if RAID_DEBUG
	if (!::IsDebuggerPresent())
		return;

	int result = ::MessageBoxA(NULL, message, "Assertion Failed!", MB_OKCANCEL | MB_DEFBUTTON2);

	if (result == IDOK)
	{
		::DebugBreak();
	}
#endif
}

Logger* LogSystem::CreateLogger(const char* name, const std::wstring& path)
{
    if (Logger* logger = new Logger())
    {
        logger->Init(name, path);
        return logger;
    }
    
    return nullptr;
}

void LogSystem::ResetTickCount()
{
	sm_TickCount = 0;
}

int64_t LogSystem::GetTickCount()
{
	return sm_TickCount;
}

void LogSystem::Tick()
{
	sm_TickCount++;
}

} // namespace raid