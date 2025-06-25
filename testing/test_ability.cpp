#include "pch.h"
#include "test.h"

#include "engine/game/ability.h"

TEST(AbilityTest, AbilityComponent)
{
	using namespace raid;

	Entity e;
	AbilityComponent& abilityComp = e.AddComponent<AbilityComponent>();

	const AbilityId TestAbilityId = 1337;

	AbilityDefinition abilityDef
	{
		.Id = TestAbilityId,
		.CostType = PowerType::Mana,
		.CostValue = 15,
		.Range = 3,
		.CastWhileMoving = true,
		.Damage = 100
	};

	abilityComp.AddAbility(abilityDef);
	
	Ability* a = abilityComp.GetAbility(TestAbilityId);
	EXPECT_NE(a, nullptr);
	EXPECT_EQ(a->GetId(), TestAbilityId);
	EXPECT_EQ(a->GetCostType(), PowerType::Mana);
	EXPECT_EQ(a->GetCostValue(), 15);
	EXPECT_EQ(a->GetRange(), 3);
	EXPECT_EQ(a->CanCastWhileMoving(), true);
	EXPECT_EQ(a->GetDamage(), 100);
}