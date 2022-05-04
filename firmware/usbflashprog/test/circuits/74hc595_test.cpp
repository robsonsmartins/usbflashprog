// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @file test/circuits/74hc595_test.cpp
 * @brief Implementation of Unit Test for 74xx595 Shift Register Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "74hc595_test.hpp"

#include <chrono>

HC595 HC595Test::hc595_ = HC595();

HC595Test::HC595Test() {}

HC595Test::~HC595Test() {}

void HC595Test::SetUp() {
    hc595_.configure(1, 2, 3, 4, 5);
}

void HC595Test::TearDown() {}

bool HC595Test::compareData(const HC595::TData& a,
                            const HC595::TData& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

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
    EXPECT_TRUE(compareData(hc595_.getData(),
                            {0x5A}));
    hc595_.writeByte(0x37);
    EXPECT_TRUE(compareData(hc595_.getData(),
                            {0x37}));
    hc595_.writeWord(0xA534);
    EXPECT_TRUE(compareData(hc595_.getData(),
                            {0x34, 0xA5}));
    hc595_.writeDWord(0x8E743921);
    EXPECT_TRUE(compareData(hc595_.getData(),
                            {0x21, 0x39, 0x74, 0x8E}));
    hc595_.clear();
    EXPECT_TRUE(compareData(hc595_.getData(),
                            {0x00, 0x00, 0x00, 0x00}));
    uint8_t buf[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
    hc595_.writeData(buf, 6);
    EXPECT_TRUE(compareData(hc595_.getData(),
                            {0x01, 0x02, 0x03, 0x04, 0x05, 0x06}));
    hc595_.clear();
    EXPECT_EQ(hc595_.getBit(6), false);
    hc595_.setBit(6);
    EXPECT_EQ(hc595_.getBit(6), true);
    EXPECT_EQ(hc595_.getBit(29), false);
    hc595_.setBit(29);
    EXPECT_EQ(hc595_.getBit(29), true);
    EXPECT_TRUE(compareData(hc595_.getData(),
                            {0x40, 0x00, 0x00, 0x20, 0x00, 0x00}));
    hc595_.setBit(6, false);
    hc595_.resetBit(29);
    EXPECT_EQ(hc595_.getBit(6), false);
    EXPECT_EQ(hc595_.getBit(29), false);
    EXPECT_TRUE(compareData(hc595_.getData(),
                            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
    hc595_.toggleBit(6);
    hc595_.toggleBit(29);
    EXPECT_TRUE(compareData(hc595_.getData(),
                            {0x40, 0x00, 0x00, 0x20, 0x00, 0x00}));
    hc595_.toggleBit(6);
    hc595_.toggleBit(29);
    EXPECT_TRUE(compareData(hc595_.getData(),
                            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
}

TEST_F(HC595Test, pulse_time) {
    constexpr uint PULSE_TIME = 10000;
    constexpr uint BYTES_NUM = 8;
    constexpr uint BITS_PER_BYTE = 8;
    constexpr uint PERCENT_TOLERANCE = 20;
    hc595_.configure(1, 2, 3, 4, 5, PULSE_TIME);
    uint8_t buf[BYTES_NUM] =
        {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    auto start = std::chrono::high_resolution_clock::now();
    hc595_.writeData(buf, BYTES_NUM);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    EXPECT_EQ(duration.count() / PERCENT_TOLERANCE,
        (PULSE_TIME * 2 * BITS_PER_BYTE + PULSE_TIME) * BYTES_NUM / 1000
         / PERCENT_TOLERANCE);
}
