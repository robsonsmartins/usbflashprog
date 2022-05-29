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
 * @file test/circuits/dc2dc_test.cpp
 * @brief Implementation of Unit Test for DC To DC Converter Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "dc2dc_test.hpp"

#include "mock/hardware/adc.h"

// ---------------------------------------------------------------------------

Dc2Dc Dc2DcTest::dc2dc_ = Dc2Dc();
Dc2DcConfig Dc2DcTest::config_ = Dc2DcConfig();

// ---------------------------------------------------------------------------

Dc2DcTest::Dc2DcTest() {}

Dc2DcTest::~Dc2DcTest() {}

void Dc2DcTest::SetUp() {
    config_.pwmPin = 0;
    config_.adcChannel = 0;
    dc2dc_.configure(config_);
}

void Dc2DcTest::TearDown() {}

// ---------------------------------------------------------------------------

float Dc2DcTest::calculate_(uint16_t value, float vref) {
    float result = value * vref / (1 << 12);
    return result;
}

// ---------------------------------------------------------------------------

TEST_F(Dc2DcTest, start_stop) {
    EXPECT_EQ(dc2dc_.isRunning(), false);
    EXPECT_EQ(dc2dc_.start(), true);
    EXPECT_EQ(dc2dc_.isRunning(), true);
    EXPECT_EQ(dc2dc_.stop(), true);
    EXPECT_EQ(dc2dc_.isRunning(), false);
}

TEST_F(Dc2DcTest, set_get_v_duty) {
    EXPECT_EQ(dc2dc_.isRunning(), false);
    dc2dc_.adjust();
    EXPECT_EQ(dc2dc_.start(), true);
    EXPECT_EQ(dc2dc_.isRunning(), true);
    EXPECT_EQ(dc2dc_.getVTarget(), 0.0f);
    EXPECT_EQ(dc2dc_.getDuty(), 0.0f);
    EXPECT_EQ(dc2dc_.getV(), 0.0f);
    dc2dc_.setV(12.0f);
    EXPECT_EQ(dc2dc_.getVTarget(), 12.0f);
    float vActual =
        calculate_((kRawAdcData[0] + kRawAdcData[1]) / 2, config_.adcVref);
    dc2dc_.adjust();
    EXPECT_NEAR(dc2dc_.getV(), vActual, 0.2f);
    dc2dc_.setV(vActual * 1.06f);
    dc2dc_.adjust();
    EXPECT_NEAR(dc2dc_.getV(), vActual * 1.06f, 0.2f);
    dc2dc_.setV(vActual * 0.94f);
    dc2dc_.adjust();
    EXPECT_NEAR(dc2dc_.getV(), vActual * 0.94f, 0.2f);
    dc2dc_.setV(0.1f);
    dc2dc_.adjust();
    EXPECT_NEAR(dc2dc_.getV(), vActual, 0.2f);
    EXPECT_EQ(dc2dc_.getCalibration(), 0.0f);
    dc2dc_.setCalibration(1.0f);
    EXPECT_EQ(dc2dc_.getCalibration(), 1.0f);
}

TEST_F(Dc2DcTest, constructor_config) {
    EXPECT_EQ(dc2dc_.getConfig(), config_);
    Dc2DcConfig newConfig(2, 3, 2.0f, 1.0f, 20000, 2.5f);
    dc2dc_.configure(newConfig);
    EXPECT_NE(dc2dc_.getConfig(), config_);
    EXPECT_EQ(dc2dc_.getConfig(), newConfig);
    Dc2DcConfig newConfig2(config_);
    Dc2Dc newDc2Dc(newConfig2);
    EXPECT_EQ(newDc2Dc.getConfig(), newConfig2);
}
