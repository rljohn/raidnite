#include "pch.h"

#include "engine/game/combat.h"
#include "engine/game/damage.h"
#include "engine/game/world.h"
#include "engine/unit/spawner.h"
#include "engine/unit/unit.h"
#include "engine/game/world.h"

using namespace raid;

TEST(FactionTest, FactionBasics)
{
	Unit a;
	a.GetAttributes().SetupAttributes();

	Unit b;
	b.GetAttributes().SetupAttributes();
	
	FactionId testGoodGuys = 1;
	FactionId testBadGuys = 2;

	FactionManager manager;
	manager.SetRelationship(testGoodGuys, testBadGuys, FactionRelationship::Hostile);

	ASSERT_NE(a.GetAttribute<AttributeType::Faction>(), nullptr);
	ASSERT_NE(b.GetAttribute<AttributeType::Faction>(), nullptr);

	if (IAttribute* fa = a.GetAttribute<AttributeType::Faction>())
	{
		fa->SetValue(testGoodGuys);
	}

	if (IAttribute* fb = b.GetAttribute<AttributeType::Faction>())
	{
		fb->SetValue(testBadGuys);
	}

	FactionId f1 = a.GetAttribute<AttributeType::Faction>()->As<FactionId>();
	FactionId f2 = b.GetAttribute<AttributeType::Faction>()->As<FactionId>();

	EXPECT_EQ(manager.GetRelationship(f1, f2), FactionRelationship::Hostile);
}