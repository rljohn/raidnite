#include "pch.h"

#include "test.h"
#include "engine/localization/localization.h"

using namespace raid;

constexpr const char* test_localization_strings = R"(
{
	"version" : 1,
	"strings" :
	[
		{
			"key" : "LOC_TEST",
			"text" : {
				"en" : "TEST STRING",
				"fr" : "TEST STRING FRANCAIS"
			}
		}
	]
}
)";

TEST(LocalizationTest, ParseJson)
{
	LocalizationSystem system;

	LocalizationSet set;
	system.AddLocalizationSet(&set);

	ASSERT_TRUE(LocalizationIO::LoadLocalizationData(test_localization_strings, system.GetLanguage(), set));

	size_t key = StringHash{}("LOC_TEST");

	// Validate english text lookup passed
	const char* en = system.GetEntry(key);
	EXPECT_STREQ(en, "TEST STRING");

	// Reset system in French
	system.SetLanguage(Language::French);
	ASSERT_TRUE(LocalizationIO::LoadLocalizationData(test_localization_strings, system.GetLanguage(), set));

	// Validate french text lookup passed
	const char* fr = system.GetEntry(key);
	EXPECT_STREQ(fr, "TEST STRING FRANCAIS");
}

TEST(LocalizationTest, LanguageChanged)
{
	LocalizationSystem system;

	LocalizationSet set;
	system.AddLocalizationSet(&set);

	bool receivedCallback = false;
	system.LanguageChanged().Register([&](Language lang)
	{
		ASSERT_TRUE(LocalizationIO::LoadLocalizationData(test_localization_strings, lang, set));
		receivedCallback = true;
	});

	system.SetLanguage(Language::French);

	size_t key = StringHash{}("LOC_TEST");
	const char* fr = system.GetEntry(key);
	EXPECT_STREQ(fr, "TEST STRING FRANCAIS");
	EXPECT_TRUE(receivedCallback);
}