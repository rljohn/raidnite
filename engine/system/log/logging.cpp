#include "engine/pch.h"
#include "engine/engine.h"
#include "engine/system/log/logging.h"
#include "engine/game/game.h"

#include "spdlog/pattern_formatter.h"
#include "spdlog/sinks/msvc_sink.h"

namespace raid
{

class LogFormatter : public spdlog::custom_flag_formatter
{
public:

	void format(const spdlog::details::log_msg&, const std::tm&, spdlog::memory_buf_t& dest) override
	{
		Frame frame = GetCurrentGameFrame();

		std::string str = std::to_string(frame);
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
		m_Loggers[0] = spdlog::basic_logger_mt(m_Name, filename);

#if MSVC_LOGGER
		m_Loggers[1] = std::make_shared<spdlog::logger>("msvc_logger", std::make_shared<spdlog::sinks::msvc_sink_mt>());
#endif

		spdlog::set_level(spdlog::level::trace);

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
	m_Loggers[0]->info("Log Started: {}", std::string(buffer));
}

void Logger::Shutdown()
{
	for (std::shared_ptr<spdlog::logger>& log : m_Loggers)
	{
		if (log)
		{
			spdlog::drop(log->name());
			log = nullptr;
		}
	}
}


Logger* LogSystem::sm_DefaultLogger = nullptr;

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
#if RAID_LOGGING
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

} // namespace raid