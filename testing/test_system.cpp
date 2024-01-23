#include "pch.h"

#include "test.h"
#include "test_system.h"
#include "engine/system/check.h"

using namespace raid;

////////////////////
// Intrusive Lists
////////////////////

TEST(ListsTest, IntrusiveTestGeneric)
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

////////////////////
// Pools
////////////////////

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

////////////////////
// Delegates
////////////////////

TEST(DelgateTest, AddRemoveDelegates)
{
	Delegate<int> dlgt;

	int count = 0;
	auto addOne = [&](int value)
	{
		count += value;
	};

	auto addTwo = [&](int value)
	{
		count += (2 * value);
	};

	auto addThree = [&](int value)
	{
		count += (3 * value);
	};

	dlgt.Register(addOne);
	dlgt.Register(addTwo);
	dlgt.Register(addThree);

	// increment count using all callbacks
	dlgt.Invoke(1);
	EXPECT_EQ(count, 6);

	// unregister the second callback and invoke again
	dlgt.Unregister(addTwo);
	dlgt.Invoke(1);
	EXPECT_EQ(count, 10);

	// unregister both functions, invoke again = no change
	dlgt.Unregister(addOne);
	dlgt.Invoke(1);
	EXPECT_EQ(count, 13);
}

TEST(DelgateTest, DoubleRegister)
{
	Delegate<int> dlgt;

	int count = 0;
	auto addOne = [&](int value)
	{
		count += value;
	};

	// register twice
	dlgt.Register(addOne);
	dlgt.Register(addOne);
	dlgt.Invoke(1);
	EXPECT_EQ(count, 2);

	// unregister should grab both
	dlgt.Unregister(addOne);
	dlgt.Invoke(1);
	EXPECT_EQ(count, 2);
}

////////////////////////////////////////
// Structured Error Handling
//////////////////// ////////////////////

TEST(SehTest, SehTestBasic)
{
	int count = 0;

	stry
	{
		count += 1;
		scheck(count == 0, label);
	}
	scatch(label)
	{
		count += 1;
		sthrowfinally();
	}
	sfinally
	{
		count += 1;
	}

	EXPECT_EQ(count, 3);
}