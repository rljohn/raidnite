#pragma once

#include "engine/types.h"
#include "engine/entity/entity.h"
#include "engine/game/combat.h"
#include "engine/game/damage.h"
#include "engine/game/game.h"
#include "engine/game/world.h"

class GameTest : public testing::Test
{
protected:

    void SetUp() override;
    void TearDown() override;

    raid::DamageCalculator m_Calculator;
    raid::World m_World;
    raid::CombatSystem m_CombatSystem;
};
