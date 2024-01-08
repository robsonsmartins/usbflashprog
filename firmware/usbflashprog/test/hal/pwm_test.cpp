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
 * @file test/hal/pwm_test.cpp
 * @brief Implementation of Unit Test for Pico PWM Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "pwm_test.hpp"

// ---------------------------------------------------------------------------

constexpr uint kSelectedGpioPin = 1;

Pwm PwmTest::pwm_ = Pwm(kSelectedGpioPin);

// ---------------------------------------------------------------------------

TEST_F(PwmTest, default_new) {
    EXPECT_EQ(pwm_.getPin(), kSelectedGpioPin);
    EXPECT_EQ(pwm_.getSlice(), kSelectedGpioPin);
    EXPECT_EQ(pwm_.getChannel(), kSelectedGpioPin);
    EXPECT_EQ(pwm_.getFreq(), Pwm::kPwmDefaultFreq);
    EXPECT_EQ(pwm_.getDuty(), 0.0f);
    EXPECT_EQ(pwm_.isRunning(), false);
}

TEST_F(PwmTest, set_duty_freq) {
    EXPECT_EQ(pwm_.getFreq(), Pwm::kPwmDefaultFreq);
    pwm_.setFreq(60000);
    EXPECT_EQ(pwm_.getFreq(), 60000);
    EXPECT_EQ(pwm_.getDuty(), 0.0f);
    pwm_.setDuty(50.1f);
    EXPECT_EQ(pwm_.getDuty(), 50.1f);
}

TEST_F(PwmTest, start_stop) {
    EXPECT_EQ(pwm_.isRunning(), false);
    pwm_.start();
    EXPECT_EQ(pwm_.isRunning(), true);
    pwm_.stop();
    EXPECT_EQ(pwm_.isRunning(), false);
    pwm_.setFreq(Pwm::kPwmDefaultFreq);
    pwm_.setDuty(50.0f);
    pwm_.start();
    EXPECT_EQ(pwm_.isRunning(), true);
    EXPECT_EQ(pwm_.getFreq(), Pwm::kPwmDefaultFreq);
    pwm_.setFreq(60000);
    EXPECT_EQ(pwm_.getFreq(), 60000);
    EXPECT_EQ(pwm_.getDuty(), 50.0f);
    pwm_.setDuty(30.0f);
    EXPECT_EQ(pwm_.getDuty(), 30.0f);
    EXPECT_EQ(pwm_.isRunning(), true);
    pwm_.stop();
    EXPECT_EQ(pwm_.isRunning(), false);
}
