#include "pch.h"

#include "test.h"

using namespace raid;

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

TEST(EncounterAbilityCast, EncounterTests)
{
	SpellId spellId = 5;
	PlayerId source = 10;
	PlayerId target = 15;
	TimeStamp now = Time::GetCurrent();

	EncounterEvent start = EncounterEvents::CreateEvent(source, target, now);
	EncounterEvents::OnAbilityStart(start, spellId);

	EXPECT_EQ(start.m_Type, EncounterEventType::AbilityStart);
	EXPECT_EQ(start.m_ExtraData1.Int64, spellId);
}

TEST(IntrusiveTestGeneric, IntrusiveLists)
{
	SpellId spellId = 5;
	PlayerId source = 10;
	PlayerId target = 15;
	TimeStamp now = Time::GetCurrent();

	inlist<EncounterEvent> events;

	EncounterEvent s1 = EncounterEvents::CreateEvent(source, target, now);
	EncounterEvents::OnAbilityStart(s1, spellId);

	EncounterEvent s2 = EncounterEvents::CreateEvent(source, target, now);
	EncounterEvents::OnAbilityStart(s2, spellId);

	EncounterEvent s3 = EncounterEvents::CreateEvent(source, target, now);
	EncounterEvents::OnAbilityStart(s3, spellId);

	events.push_back(s1.m_Node);
	events.push_back(s2.m_Node);
	events.push_back(s3.m_Node);

	EXPECT_EQ(events.size(), 3);
	
}

TEST_F(PoolTest, PoolTestGeneric)
{
	EncounterEvent evt;
	size_t offset = offsetof(EncounterEvent, m_Node);

	pool->Init(offset);
	EXPECT_EQ(pool->GetNumAvailable(), POOL_SIZE);

	EncounterEvent* e = pool->Create();
	EXPECT_EQ(pool->GetNumAvailable(), POOL_SIZE - 1);

	pool->Free(e);
	EXPECT_EQ(pool->GetNumAvailable(), POOL_SIZE);
}