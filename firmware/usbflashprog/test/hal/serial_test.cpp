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
 * @file test/hal/serial_test.cpp
 * @brief Implementation of Unit Test for Serial Communication Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <string>
#include <iostream>
#include "serial_test.hpp"
#include "mock/pico/stdlib.h"

// ---------------------------------------------------------------------------

Serial SerialTest::serial_ = Serial();

// ---------------------------------------------------------------------------

TEST_F(SerialTest, get_methods) {
    EXPECT_EQ(serial_.getChar(), PICO_ERROR_TIMEOUT);
    EXPECT_EQ(serial_.getChar(2), kStdioMockPredefinedChar);
    EXPECT_EQ(serial_.getBuf(nullptr, 0, 0), 0);
    char buf[17];
    buf[16] = 0;
    EXPECT_EQ(serial_.getBuf(buf, 16, 0), 0);
    EXPECT_EQ(serial_.getBuf(buf, 16, 2), 16);
    EXPECT_EQ(std::string(buf), std::string(16, kStdioMockPredefinedChar));
    EXPECT_EQ(serial_.getStr(), "");
    EXPECT_EQ(serial_.getInt(), 0);
    EXPECT_EQ(serial_.getInt(16), 0);
    EXPECT_EQ(serial_.getFloat(), 0.0f);
}

TEST_F(SerialTest, put_methods) {
    serial_.putChar(kStdioMockPredefinedChar, false);
    serial_.putChar(kStdioMockPredefinedChar, true);
    char buf[17];
    serial_.putBuf(buf, 16, false);
    serial_.putBuf(buf, 16, true);
    serial_.putStr(std::string(16, kStdioMockPredefinedChar), false);
    serial_.putStr(std::string(16, kStdioMockPredefinedChar), true);
    serial_.putInt(12, 10, false);
    serial_.putInt(12, 10, true);
    serial_.putFloat(1.5, false);
    serial_.putFloat(1.5, true);
}

TEST_F(SerialTest, stream_methods) {
    std::ostream &os = serial_.out();
    std::istream &is = serial_.in();
}
