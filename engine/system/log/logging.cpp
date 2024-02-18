#include "engine/pch.h"
#include "engine/system/log/logging.h"

#include "spdlog/pattern_formatter.h"
class my_formatter_flag : public spdlog::custom_flag_formatter
{
public:

	void format(const spdlog::details::log_msg&, const std::tm&, spdlog::memory_buf_t& dest) override
	{
		std::string some_txt = "custom-flag";
		dest.append(std::to_string(GameFrame));
	}

	std::unique_ptr<custom_flag_formatter> clone() const override
	{
		return spdlog::details::make_unique<my_formatter_flag>();
	}

	int GameFrame = 0;
};

namespace raid
{

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

		auto formatter = std::make_unique<spdlog::pattern_formatter>();
		formatter->add_flag<my_formatter_flag>('*').set_pattern("[%10*] [%D %r] [%l] %v");
		spdlog::set_formatter(std::move(formatter));

		//spdlog::set_pattern("[%D %r] [%l] %v");

		return true;
	}
	catch (const spdlog::spdlog_ex ex)
	{
		std::cout << "Log initialization failed: " << ex.what() << std::endl;
		return false;
	}
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

} // namespace raid