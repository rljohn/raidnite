#include "pch.h"

#include "engine/encounter/encounter_log.h"
#include "engine/encounter/encounter_serialization.h"
#include "engine/encounter/event.h"
#include "engine/game/combat.h"
#include "engine/system/fileutil.h"
#include "test.h"

using namespace raid;

TEST(EncounterAbilityCast, EncounterTests)
{
	SpellId spellId = 5;
	PlayerId source = 10;
	PlayerId target = 15;
	TimeStamp now = Time::GetCurrent();

	EncounterEvent start;
	EncounterEvents::OnAbilityStart(start, spellId);

	EXPECT_EQ(start.m_Type, EncounterEventType::AbilityStart);
	EXPECT_EQ(start.m_ExtraData1.Int64, spellId);
}

TEST(EncounterStartStop, EncounterTests)
{
	TimeStamp now = Time::GetCurrent();

	Duration empty = Duration();

	// Non-started encounter should have empty duration
	Encounter encounter;
	Duration d1 = encounter.GetDuration(now);
	EXPECT_EQ(d1, empty);

	// Begin the encounter: duraiton should remain empty
	encounter.Begin(now);
	Duration d2 = encounter.GetDuration(now);
	EXPECT_EQ(d2, empty);

	// Add 1 seconds of time and end encounter
	Time::Add(now, Seconds(1));
	encounter.End(now);

	// Duration should now be 1 second
	Duration d3 = encounter.GetDuration(now);
	Seconds s3 = Time::ToSeconds(d3);
	EXPECT_EQ(s3, Seconds(1));
}

/////////////////////////////////////////
//  EncounterLogTest
/////////////////////////////////////////

class EncounterLogTest : public testing::Test
{
protected:

	std::wstring m_LogPath;

	void SetUp() override;
	void TearDown() override;
};

void EncounterLogTest::SetUp()
{
	m_LogPath = FileUtil::GetAppDataFolder() + L"\\unittest\\encounter.log";
}

void EncounterLogTest::TearDown()
{
	FileUtil::RemoveFile(m_LogPath);
}

TEST_F(EncounterLogTest, InitShutdown)
{
	EncounterLog log;
	log.Init();
	EXPECT_EQ(Game::GameEventDlgt().GetCount(), 1);

	log.Shutdown();
	EXPECT_EQ(Game::GameEventDlgt().GetCount(), 0);
}

TEST_F(EncounterLogTest, StartEnd)
{
	EncounterLog log;

	CombatStartEvent start;
	CombatEndEvent end;

	log.OnGameEvent(&start);
	log.OnGameEvent(&end);

	log.OnGameEvent(&start);
	log.OnGameEvent(&end);

	EXPECT_EQ(log.GetEncounterList().size(), 2);
}

TEST_F(EncounterLogTest, SaveLoad)
{
	CombatSystemRAII combat;

	// Pool big enough for the two of us
	using CustomPool = IntrusivePool<EncounterEvent, 10>;
	CustomPool* pool = new CustomPool();

	EncounterLog log;
	log.Init(pool);

	EncounterSerialization serializer;
	EXPECT_FALSE(m_LogPath.empty());

	CombatStartEvent start;
	CombatEndEvent end;

	DamageEvent dmg(nullptr);
	dmg.BaseDamage = 10;
	dmg.ActualDamage = 6;
	dmg.DamageType = DamageType::Fire;

	log.OnGameEvent(&start);
	log.OnGameEvent(&dmg);
	log.OnGameEvent(&end);

	// verify the save
	EXPECT_TRUE(serializer.Save(m_LogPath, log));

	// load into a separate log
	EncounterLog second;
	second.Init(pool);
	EXPECT_TRUE(serializer.Load(m_LogPath, second));

	// verify matching logs
	EXPECT_EQ(log.GetEncounterList().size(), second.GetEncounterList().size());
}