#include "pch.h"

#include "engine/system/args.h"
#include "test.h"

using namespace raid;

TEST(ArgsTest, AllTypes)
{
	size_t argCount = CommandLineManager::GetArgCount();

	{
		BoolArgument bArg(L"boolArg");
		IntArgument iArg1(L"intArg1");
		IntArgument iArg2(L"intArg2");
		IntArgument iArg3(L"intArg3");
		StringArgument sArg1(L"strArg1");
		StringArgument sArg2(L"strArg2");
		StringArgument sArg3(L"strArg3");

		EXPECT_FALSE(bArg);
		EXPECT_EQ(iArg1, 0);
		EXPECT_EQ(iArg2, 0);
		EXPECT_STREQ(sArg1, L"");
		EXPECT_STREQ(sArg2, L"");

		EXPECT_FALSE(bArg.HasValue());
		EXPECT_FALSE(iArg1.HasValue());
		EXPECT_FALSE(iArg2.HasValue());
		EXPECT_FALSE(iArg3.HasValue());
		EXPECT_FALSE(sArg1.HasValue());
		EXPECT_FALSE(sArg2.HasValue());
		EXPECT_FALSE(sArg3.HasValue());

		const wchar_t* args = L"Game.exe -boolArg -intArg1=5 -intArg2 6 -strArg1=test -strArg2 \"test string\" -intArg3=foo -intArg3 bar -strArg3";
		CommandLineManager::Init(args);

		EXPECT_TRUE(bArg);
		EXPECT_EQ(iArg1, 5);
		EXPECT_EQ(iArg2, 6);
		EXPECT_EQ(iArg3, 0);
		EXPECT_STREQ(sArg1, L"test");
		EXPECT_STREQ(sArg2, L"test string");
		EXPECT_STREQ(sArg3, L"");

		EXPECT_TRUE(bArg.GetValue());
		EXPECT_EQ(iArg1.GetValue(), 5);
		EXPECT_EQ(iArg2.GetValue(), 6);
		EXPECT_STREQ(sArg1.GetValue().c_str(), L"test");
		EXPECT_STREQ(sArg2.GetValue().c_str(), L"test string");

		EXPECT_TRUE(bArg.HasValue());
		EXPECT_TRUE(iArg1.HasValue());
		EXPECT_TRUE(iArg2.HasValue());
		EXPECT_FALSE(iArg3.HasValue());
		EXPECT_TRUE(sArg1.HasValue());
		EXPECT_TRUE(sArg2.HasValue());
		EXPECT_FALSE(sArg3.HasValue());
	}

	// all arguments unregistered
	EXPECT_EQ(argCount, CommandLineManager::GetArgCount());
}