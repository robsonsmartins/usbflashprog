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
 * @file test/circuits/74hc165_test.cpp
 * @brief Implementation of Unit Test for 74xx165 Shift Register Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "74hc165_test.hpp"
#include "mock/hardware/gpio.h"

#include <chrono>  // NOLINT

// ---------------------------------------------------------------------------

HC165 HC165Test::hc165_ = HC165();

// ---------------------------------------------------------------------------

HC165Test::HC165Test() {}

HC165Test::~HC165Test() {}

void HC165Test::SetUp() {
    hc165_.configure(1, 2, 3, 4, 5);
}

void HC165Test::TearDown() {}

// ---------------------------------------------------------------------------

TEST_F(HC165Test, ce) {
    EXPECT_EQ(hc165_.getCE(), false);
    hc165_.chipEnable();
    EXPECT_EQ(hc165_.getCE(), true);
    hc165_.chipEnable(false);
    EXPECT_EQ(hc165_.getCE(), false);
    hc165_.chipEnable();
    EXPECT_EQ(hc165_.getCE(), true);
    hc165_.chipDisable();
    EXPECT_EQ(hc165_.getCE(), false);
}

TEST_F(HC165Test, data) {
    hc165_.load();
    EXPECT_EQ(hc165_.getBit(), false);
    EXPECT_EQ(hc165_.getBit(7), false);
    EXPECT_EQ(hc165_.getBit(15), false);
    EXPECT_EQ(hc165_.readByte(), 0x00);
    EXPECT_EQ(hc165_.readWord(), 0x0000);
    EXPECT_EQ(hc165_.readDWord(), 0x00000000);

    gpio_put(4, true);
    EXPECT_EQ(hc165_.getBit(), true);
    EXPECT_EQ(hc165_.getBit(7), true);
    EXPECT_EQ(hc165_.getBit(15), true);
    EXPECT_EQ(hc165_.readByte(), 0xFF);
    EXPECT_EQ(hc165_.readWord(), 0xFFFF);
    EXPECT_EQ(hc165_.readDWord(), 0xFFFFFFFF);

    EXPECT_EQ(hc165_.readByte(true), 0xFF);
    EXPECT_EQ(hc165_.readWord(true), 0xFFFF);
    EXPECT_EQ(hc165_.readDWord(true), 0xFFFFFFFF);

    HC165 newHc165 = HC165(5, 4, 3, 2, 1, 20);
    hc165_.load();
    gpio_put(2, false);
    EXPECT_EQ(newHc165.getBit(), false);

    HC165 otherHc165 = HC165();
    otherHc165.load();
    EXPECT_EQ(otherHc165.getBit(), false);
    EXPECT_EQ(otherHc165.readByte(), 0x00);
    EXPECT_EQ(otherHc165.readWord(), 0x0000);
    EXPECT_EQ(otherHc165.readDWord(), 0x00000000);

    otherHc165.configure(1, 2, 0xFF, 0xFF, 4);
    gpio_put(4, false);
    EXPECT_EQ(otherHc165.getBit(), true);
    EXPECT_EQ(otherHc165.readByte(), 0xFF);
}

TEST_F(HC165Test, pulse_time) {
    constexpr uint kPulseTime = 10000;
    constexpr uint kNumBytes = 8;
    constexpr uint kBitsPerByte = 8;
    hc165_.configure(1, 2, 3, 4, 5, kPulseTime);
    uint8_t buf[kNumBytes];
    auto start = std::chrono::high_resolution_clock::now();
    hc165_.readData(buf, kNumBytes);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    EXPECT_NEAR(duration.count(),
                (kPulseTime * 2 * kBitsPerByte + kPulseTime) * kNumBytes / 1000,
                100.0f);
}
