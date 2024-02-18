#include "pch.h"

#include "engine/system/fileutil.h"
#include "engine/system/log/logging.h"

using namespace raid;

class LoggingTest : public testing::Test
{
protected:

	std::wstring m_LogPath;

	void SetUp() override;
	void TearDown() override;
};

void LoggingTest::SetUp()
{
	m_LogPath = FileUtil::GetAppDataFolder() + L"\\unittest\\game.log";
}

void LoggingTest::TearDown()
{
	FileUtil::RemoveFile(m_LogPath);
}

TEST_F(LoggingTest, LoggingBasics)
{
	Logger* logger = LogSystem::CreateLogger("LogTest", m_LogPath);
	ASSERT_NE(logger, nullptr);

	LogChannel channel("Test Channel", LogSeverity::Display);

	// display and error message should succeed
	EXPECT_TRUE(logger->LogMessage(channel, LogSeverity::Fatal, "Test String: {}", "Please Ignore"));
	EXPECT_TRUE(logger->LogMessage(channel, LogSeverity::Display, "Test String: {}", "Please Ignore"));

	// verbose message should fail
	EXPECT_FALSE(logger->LogMessage(channel, LogSeverity::Verbose, "Test String: {}", "Please Ignore"));

	// logger is shutdown, cannot log any more
	logger->Shutdown();

	EXPECT_FALSE(logger->LogMessage(channel, LogSeverity::Display, "Test String: {}", "Please Ignore"));
	delete logger;
}

TEST_F(LoggingTest, LogRAII)
{
	Logger* logger = LogSystem::CreateLogger("LogTest", m_LogPath);
	ASSERT_NE(logger, nullptr);

	{
		LoggerRAII raii(logger);
	}
	
	EXPECT_EQ(LogSystem::GetDefaultLogger(), nullptr);
}

TEST_F(LoggingTest, LogMacros)
{
	Logger* logger = LogSystem::CreateLogger("LogTest", m_LogPath);
	ASSERT_NE(logger, nullptr);

	{
		LoggerRAII raii(logger);
		mainDisplay("Test String!");
		mainDisplay("Test String: {}", "Please Ignore");

		bool success = false;
		mainAssert(success, "Condition failed");
	}
}