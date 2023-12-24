#include "pch.h"

#include "engine/types.h"
#include "engine/encounter/encounter.h"
#include "engine/system/time.h"

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