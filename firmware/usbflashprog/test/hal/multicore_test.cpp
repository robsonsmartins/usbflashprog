// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup UnitTests
 * @file test/hal/multicore_test.cpp
 * @brief Implementation of Unit Test for Pico Multi Core Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "multicore_test.hpp"

// ---------------------------------------------------------------------------

constexpr uint64_t kWaitTime = 10;  // in usec

void second_core_entry_(MultiCore& core);  // NOLINT

MultiCore MultiCoreTest::multicore_ = MultiCore(second_core_entry_);

// ---------------------------------------------------------------------------

TEST_F(MultiCoreTest, launch_stop) {
    EXPECT_EQ(multicore_.isRunning(), false);
    EXPECT_EQ(multicore_.getStatus(), MultiCore::csStopped);
    multicore_.launch();
    multicore_.usleep(kWaitTime);
    EXPECT_EQ(multicore_.isRunning(), true);
    EXPECT_EQ(multicore_.getStatus(), MultiCore::csRunning);
    multicore_.putParam(20);
    multicore_.usleep(kWaitTime);
    EXPECT_EQ(multicore_.getParam(), 40);
    multicore_.putParam(50);
    multicore_.usleep(kWaitTime);
    EXPECT_EQ(multicore_.getParam(), 100);
    multicore_.putParam(10);
    multicore_.stop();
    multicore_.usleep(kWaitTime);
    EXPECT_EQ(multicore_.isRunning(), false);
    EXPECT_EQ(multicore_.getStatus(), MultiCore::csStopped);
    multicore_.putParam(60);
    multicore_.usleep(kWaitTime);
    EXPECT_EQ(multicore_.getParam(), 0);
    multicore_.msleep(1);
}

// ---------------------------------------------------------------------------

void second_core_entry_(MultiCore& core) {  // NOLINT
    uintptr_t p = 0;
    do {
        p = core.getParam();
        core.putParam(p * 2);
        core.usleep(1);
        core.lock();
        core.unlock();
    } while (p != 0 && !core.isStopRequested());
}
