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
 * @file test/circuits/74hc595_test.cpp
 * @brief Implementation of Unit Test for 74xx595 Shift Register Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "74hc595_test.hpp"

#include <chrono> // NOLINT

// ---------------------------------------------------------------------------

HC595 HC595Test::hc595_ = HC595();

// ---------------------------------------------------------------------------

HC595Test::HC595Test() {}

HC595Test::~HC595Test() {}

void HC595Test::SetUp() {
    hc595_.configure(1, 2, 3, 4, 5);
}

void HC595Test::TearDown() {}

bool HC595Test::compareData_(const HC595::TData& a,
                             const HC595::TData& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// ---------------------------------------------------------------------------

TEST_F(HC595Test, oe) {
    EXPECT_EQ(hc595_.getOE(), false);
    hc595_.outputEnable();
    EXPECT_EQ(hc595_.getOE(), true);
    hc595_.outputEnable(false);
    EXPECT_EQ(hc595_.getOE(), false);
    hc595_.outputEnable();
    EXPECT_EQ(hc595_.getOE(), true);
    hc595_.outputDisable();
    EXPECT_EQ(hc595_.getOE(), false);
}

TEST_F(HC595Test, data) {
    EXPECT_EQ(hc595_.getData().size(), 0);
    hc595_.writeByte(0x5A);
    EXPECT_EQ(hc595_.getData().size(), 1);
    EXPECT_TRUE(compareData_(hc595_.getData(),
                            {0x5A}));
    hc595_.writeByte(0x37);
    EXPECT_TRUE(compareData_(hc595_.getData(),
                            {0x37}));
    hc595_.writeWord(0x37);
    EXPECT_TRUE(compareData_(hc595_.getData(),
                            {0x37}));
    hc595_.writeDWord(0x37);
    EXPECT_TRUE(compareData_(hc595_.getData(),
                            {0x37}));
    hc595_.writeWord(0xA534);
    EXPECT_TRUE(compareData_(hc595_.getData(),
                            {0x34, 0xA5}));
    hc595_.writeDWord(0xA534);
    EXPECT_TRUE(compareData_(hc595_.getData(),
                            {0x34, 0xA5}));
    hc595_.writeDWord(0x8E743921);
    EXPECT_TRUE(compareData_(hc595_.getData(),
                            {0x21, 0x39, 0x74, 0x8E}));
    hc595_.clear();
    EXPECT_EQ(hc595_.getData().size(), 0);

    hc595_.writeDWord(0x0123FFFE);
    EXPECT_TRUE(compareData_(hc595_.getData(),
                            {0xFE, 0xFF, 0x23, 0x01}));
    hc595_.increment();
    EXPECT_TRUE(compareData_(hc595_.getData(),
                            {0xFF, 0xFF, 0x23, 0x01}));
    hc595_.increment();
    EXPECT_TRUE(compareData_(hc595_.getData(),
                            {0x00, 0x00, 0x24, 0x01}));
    hc595_.increment();
    EXPECT_TRUE(compareData_(hc595_.getData(),
                            {0x01, 0x00, 0x24, 0x01}));

    hc595_.clear();
    EXPECT_EQ(hc595_.getData().size(), 0);

    hc595_.writeDWord(0x23FFFE);
    EXPECT_TRUE(compareData_(hc595_.getData(),
                            {0xFE, 0xFF, 0x23}));
    hc595_.increment();
    EXPECT_TRUE(compareData_(hc595_.getData(),
                            {0xFF, 0xFF, 0x23}));
    hc595_.increment();
    EXPECT_TRUE(compareData_(hc595_.getData(),
                            {0x00, 0x00, 0x24}));
    hc595_.increment();
    EXPECT_TRUE(compareData_(hc595_.getData(),
                            {0x01, 0x00, 0x24}));

    hc595_.clear();
    EXPECT_EQ(hc595_.getData().size(), 0);

    uint8_t buf[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
    hc595_.writeData(buf, 6);
    EXPECT_TRUE(compareData_(hc595_.getData(),
                            {0x01, 0x02, 0x03, 0x04, 0x05, 0x06}));
    hc595_.clear();
    EXPECT_EQ(hc595_.getBit(6), false);
    hc595_.setBit(6);
    EXPECT_EQ(hc595_.getBit(6), true);
    EXPECT_EQ(hc595_.getBit(29), false);
    hc595_.setBit(29);
    EXPECT_EQ(hc595_.getBit(29), true);
    EXPECT_TRUE(compareData_(hc595_.getData(),
                            {0x40, 0x00, 0x00, 0x20}));
    hc595_.setBit(6, false);
    hc595_.resetBit(29);
    EXPECT_EQ(hc595_.getBit(6), false);
    EXPECT_EQ(hc595_.getBit(29), false);
    EXPECT_TRUE(compareData_(hc595_.getData(),
                            {0x00, 0x00, 0x00, 0x00}));
    hc595_.toggleBit(6);
    hc595_.toggleBit(29);
    EXPECT_TRUE(compareData_(hc595_.getData(),
                            {0x40, 0x00, 0x00, 0x20}));
    hc595_.toggleBit(6);
    hc595_.toggleBit(29);
    EXPECT_TRUE(compareData_(hc595_.getData(),
                            {0x00, 0x00, 0x00, 0x00}));
    HC595 newHc595 = HC595(5, 4, 3, 2, 1, 20);
    EXPECT_EQ(newHc595.getData().size(), 0);
    newHc595.setBit(5);
    EXPECT_EQ(newHc595.getData().size(), 1);
    newHc595.toggleBit(9);
    EXPECT_EQ(newHc595.getData().size(), 2);
}

TEST_F(HC595Test, pulse_time) {
    constexpr uint kPulseTime = 10000;
    constexpr uint kNumBytes = 8;
    constexpr uint kBitsPerByte = 8;
    hc595_.configure(1, 2, 3, 4, 5, kPulseTime);
    uint8_t buf[kNumBytes] =
        {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    auto start = std::chrono::high_resolution_clock::now();
    hc595_.writeData(buf, kNumBytes);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    EXPECT_NEAR(
        duration.count(),
        (kPulseTime * 2 * kBitsPerByte + kPulseTime) * kNumBytes / 1000,
        100.0f);
}
