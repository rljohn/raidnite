#include "pch.h"

#include "engine/encounter/encounter_data.h"
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

	EncounterEvent start;
	EncounterEvents::OnAbilityStart(start, spellId);

	EXPECT_EQ(start.m_Type, EncounterEventType::AbilityStart);
	EXPECT_EQ(start.m_ExtraData1.Int64, spellId);
}

TEST(EncounterStartStop, EncounterTests)
{
	Frame now = 1;

	// Non-started encounter should have empty duration
	Encounter encounter;
	Frame d1 = encounter.GetDuration(now);
	EXPECT_EQ(d1, 0);

	// Begin the encounter: duration should remain empty
	encounter.Begin(now, false);
	Frame d2 = encounter.GetDuration(now);
	EXPECT_EQ(d2, 0);

	// Add 10 frames and end encounter
	now += 10;
	encounter.End(now);

	// Duration should now be 10 frames.
	// Engine can convert this to time.
	Frame d3 = encounter.GetDuration(now);
	EXPECT_EQ(d3, 10);
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

TEST_F(EncounterLogTest, StartEnd)
{
	EncounterLog log;

	CombatStartEvent start;
	CombatEndEvent end;

	log.OnGameEvent(start);
	log.OnGameEvent(end);

	log.OnGameEvent(start);
	log.OnGameEvent(end);

	EXPECT_EQ(log.GetEncounterList().size(), 2);
}

TEST_F(EncounterLogTest, SaveLoad)
{
	CombatSystemRAII combat;

	// Pool big enough for the two of us
	using CustomPool = IntrusivePool<EncounterEvent, 10>;

	EncounterLog log;
	log.Init(std::make_unique<CustomPool>());

	EncounterSerialization serializer;
	EXPECT_FALSE(m_LogPath.empty());

	CombatStartEvent start;
	CombatEndEvent end;

	DamageEvent dmg(nullptr);
	dmg.BaseDamage = 10;
	dmg.ActualDamage = 6;
	dmg.DamageType = DamageType::Fire;

	log.OnGameEvent(start);
	log.OnGameEvent(dmg);
	log.OnGameEvent(end);

	// verify the save
	EXPECT_TRUE(serializer.Save(m_LogPath, log));

	// load into a separate log
	EncounterLog second;
	second.Init(std::make_unique<CustomPool>());
	EXPECT_TRUE(serializer.Load(m_LogPath, second));

	// verify matching logs
	EXPECT_EQ(log.GetEncounterList().size(), second.GetEncounterList().size());

	log.Shutdown();
}

template<typename T, typename... Args>
void TestPackingField(Args... args)
{
	EncounterField field = {0};
	T source(std::forward<Args>(args)...);
	EncounterData::PackageField(source, field);

	T result = T();
	EncounterData::UnpackageField(field, result);
	EXPECT_EQ(source, result);
}

template<typename T, typename... Args>
void TestPackingData(Args... args)
{
	EncounterEvent data;
	T source(std::forward<Args>(args)...);
	EncounterData::PackageData(source, data);

	T result = T();
	EncounterData::UnpackageData(data, result);
	EXPECT_EQ(source, result);
}

TEST_F(EncounterLogTest, DataPacking)
{
	TestPackingField<PositionVector2D>(45, 64);
	TestPackingField<PositionVector2D>(1337, 92346);

	TestPackingField<bool>(false);
	TestPackingField<bool>(true);
	
	TestPackingData<TilePropertiesChangedEvent>(Position(5, 5), true, false, false);
	TestPackingData<TilePropertiesChangedEvent>(Position(10, 10), true, false, true);
	TestPackingData<TilePropertiesChangedEvent>(Position(10, 10), true, true, false);
	TestPackingData<TilePropertiesChangedEvent>(Position(15, 15), true, true, true);
}