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

    static const int POOL_SIZE = 1 * 1000;
    raid::IntrusivePool<raid::EncounterEvent, POOL_SIZE>* pool;

    void SetUp() override
    {
        pool = new raid::IntrusivePool<raid::EncounterEvent, POOL_SIZE>();
    }

    void TearDown() override
    {
        delete pool;
    }
};