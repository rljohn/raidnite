#pragma once

#include "engine/types.h"
#include "engine/encounter/encounter.h"
#include "engine/encounter/event.h"
#include "engine/system/delegate.h"
#include "engine/system/time.h"
#include "engine/system/inlist.h"
#include "engine/system/pool.h"

class PoolTest : public testing::Test
{
protected:

    static constexpr const int POOL_SIZE = 1000;
    raid::IPool<raid::EncounterEvent>* m_Pool;

    void SetUp() override;
    void TearDown() override;
};