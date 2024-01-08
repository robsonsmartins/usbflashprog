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
 * @file test/hal/gpio_test.cpp
 * @brief Implementation of Unit Test for Pico GPIO Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "gpio_test.hpp"

// ---------------------------------------------------------------------------

Gpio GpioTest::gpio_ = Gpio();

// ---------------------------------------------------------------------------

TEST_F(GpioTest, get_set_reset_toggle_pin) {
    EXPECT_EQ(gpio_.getPin(1), false);
    gpio_.setPin(1);
    EXPECT_EQ(gpio_.getPin(1), true);
    gpio_.setPin(1, false);
    EXPECT_EQ(gpio_.getPin(1), false);
    EXPECT_EQ(gpio_.getPin(2), false);
    gpio_.setPin(2);
    EXPECT_EQ(gpio_.getPin(2), true);
    gpio_.setPin(2, false);
    EXPECT_EQ(gpio_.getPin(2), false);
    gpio_.setPin(1);
    EXPECT_EQ(gpio_.getPin(1), true);
    gpio_.resetPin(1);
    EXPECT_EQ(gpio_.getPin(1), false);
    gpio_.togglePin(1);
    EXPECT_EQ(gpio_.getPin(1), true);
    gpio_.togglePin(1);
    EXPECT_EQ(gpio_.getPin(1), false);
}

TEST_F(GpioTest, pullup_pulldown) {
    EXPECT_EQ(gpio_.isPulledDown(1), false);
    EXPECT_EQ(gpio_.isPulledUp(1), false);
    gpio_.pullUp(1);
    EXPECT_EQ(gpio_.isPulledDown(1), false);
    EXPECT_EQ(gpio_.isPulledUp(1), true);
    gpio_.pullDown(1);
    EXPECT_EQ(gpio_.isPulledDown(1), true);
    EXPECT_EQ(gpio_.isPulledUp(1), true);
    gpio_.pullUp(1, false);
    EXPECT_EQ(gpio_.isPulledDown(1), true);
    EXPECT_EQ(gpio_.isPulledUp(1), false);
    gpio_.pullDown(1, false);
    EXPECT_EQ(gpio_.isPulledDown(1), false);
    EXPECT_EQ(gpio_.isPulledUp(1), false);
    gpio_.pullUp(2, true);
    gpio_.pullDown(2, true);
    EXPECT_EQ(gpio_.isPulledDown(1), false);
    EXPECT_EQ(gpio_.isPulledUp(1), false);
    EXPECT_EQ(gpio_.isPulledDown(2), true);
    EXPECT_EQ(gpio_.isPulledUp(2), true);
    gpio_.disablePulls(2);
    EXPECT_EQ(gpio_.isPulledDown(1), false);
    EXPECT_EQ(gpio_.isPulledUp(1), false);
    EXPECT_EQ(gpio_.isPulledDown(2), false);
    EXPECT_EQ(gpio_.isPulledUp(2), false);
}
