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
 * @file test/modules/vgenerator_test.cpp
 * @brief Implementation of Unit Test for Voltage Generator Classes.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "vgenerator_test.hpp"

#include "mock/hardware/adc.h"

// ---------------------------------------------------------------------------

VddGenerator VGeneratorTest::vdd_ = VddGenerator();
VppGenerator VGeneratorTest::vpp_ = VppGenerator();
VddConfig VGeneratorTest::vddConfig_ = VddConfig();
VppConfig VGeneratorTest::vppConfig_ = VppConfig();

// ---------------------------------------------------------------------------

VGeneratorTest::VGeneratorTest() {}

VGeneratorTest::~VGeneratorTest() {}

void VGeneratorTest::SetUp() {
    vddConfig_.pwmPin = 0;
    vddConfig_.adcChannel = 0;
    vddConfig_.ctrlPin = 1;
    vddConfig_.onVppPin = 2;
    vdd_.configure(vddConfig_);
    vppConfig_.pwmPin = 3;
    vppConfig_.adcChannel = 1;
    vppConfig_.ctrlPin = 4;
    vppConfig_.vcSinPin = 5;
    vppConfig_.vcClkPin = 6;
    vppConfig_.vcClrPin = 7;
    vppConfig_.vcRckPin = 8;
    vpp_.configure(vppConfig_);
}

void VGeneratorTest::TearDown() {}

// ---------------------------------------------------------------------------

float VGeneratorTest::calculate_(uint16_t value, float vref) {
    float result = value * vref / (1 << 12);
    return result;
}

// ---------------------------------------------------------------------------

TEST_F(VGeneratorTest, start_stop) {
    {
        EXPECT_EQ(vdd_.isRunning(), false);
        EXPECT_EQ(vdd_.start(), true);
        EXPECT_EQ(vdd_.isRunning(), true);
        vdd_.stop();
        EXPECT_EQ(vdd_.isRunning(), false);
    }
    {
        EXPECT_EQ(vpp_.isRunning(), false);
        EXPECT_EQ(vpp_.start(), true);
        EXPECT_EQ(vpp_.isRunning(), true);
        vpp_.stop();
        EXPECT_EQ(vpp_.isRunning(), false);
    }
}

TEST_F(VGeneratorTest, set_get_v_duty) {
    {
        EXPECT_EQ(vdd_.isRunning(), false);
        EXPECT_EQ(vdd_.start(), true);
        EXPECT_EQ(vdd_.isRunning(), true);
        EXPECT_EQ(vdd_.getVTarget(), 0.0f);
        EXPECT_EQ(vdd_.getDuty(), 0.0f);
        EXPECT_EQ(vdd_.getV(), 0.0f);
        vdd_.setV(5.0f);
        EXPECT_EQ(vdd_.getVTarget(), 5.0f);
        float vActual =
            calculate_((RAW_ADC_DATA[0] + RAW_ADC_DATA[1]) / 2,
                       vddConfig_.adcVref);
        vdd_.adjust();
        EXPECT_NEAR(vdd_.getV(), vActual, 0.2f);
        vdd_.setV(vActual * 1.06f);
        vdd_.adjust();
        EXPECT_NEAR(vdd_.getV(), vActual * 1.06f, 0.2f);
        vdd_.setV(vActual * 0.94f);
        vdd_.adjust();
        EXPECT_NEAR(vdd_.getV(), vActual * 0.94f, 0.2f);
        vdd_.setV(0.1f);
        vdd_.adjust();
        EXPECT_NEAR(vdd_.getV(), vActual, 0.2f);
    }
    {
        EXPECT_EQ(vpp_.isRunning(), false);
        EXPECT_EQ(vpp_.start(), true);
        EXPECT_EQ(vpp_.isRunning(), true);
        EXPECT_EQ(vpp_.getVTarget(), 0.0f);
        EXPECT_EQ(vpp_.getDuty(), 0.0f);
        EXPECT_EQ(vpp_.getV(), 0.0f);
        vpp_.setV(12.0f);
        EXPECT_EQ(vpp_.getVTarget(), 12.0f);
        float vActual =
            calculate_((RAW_ADC_DATA[0] + RAW_ADC_DATA[1]) / 2,
                       vppConfig_.adcVref);
        vpp_.adjust();
        EXPECT_NEAR(vpp_.getV(), vActual, 0.2f);
        vpp_.setV(vActual * 1.06f);
        vpp_.adjust();
        EXPECT_NEAR(vpp_.getV(), vActual * 1.06f, 0.2f);
        vpp_.setV(vActual * 0.94f);
        vpp_.adjust();
        EXPECT_NEAR(vpp_.getV(), vActual * 0.94f, 0.2f);
        vpp_.setV(0.1f);
        vpp_.adjust();
        EXPECT_NEAR(vpp_.getV(), vActual, 0.2f);
    }
}

TEST_F(VGeneratorTest, constructor_config) {
    {
        VGenerator *newVGenerator2 = new VGenerator();
        delete newVGenerator2;
    }
    {
        EXPECT_EQ(vdd_.getConfig(), vddConfig_);
        VddConfig newConfig(2, 3, 4, 5, 2.0f);
        vdd_.configure(newConfig);
        EXPECT_NE(vdd_.getConfig(), vddConfig_);
        EXPECT_EQ(vdd_.getConfig(), newConfig);
        VddConfig newConfig2(vddConfig_);
        VddGenerator newVddGenerator(newConfig2);
        EXPECT_EQ(newVddGenerator.getConfig(), newConfig2);
        VddGenerator *newVddGenerator2 = new VddGenerator();
        EXPECT_NE(newVddGenerator2->getConfig(), vddConfig_);
        delete newVddGenerator2;
    }
    {
        EXPECT_EQ(vpp_.getConfig(), vppConfig_);
        VppConfig newConfig(2, 3, 4, 5 , 6, 7, 2.0f);
        vpp_.configure(newConfig);
        EXPECT_NE(vpp_.getConfig(), vppConfig_);
        EXPECT_EQ(vpp_.getConfig(), newConfig);
        VppConfig newConfig2(vppConfig_);
        VppGenerator newVppGenerator(newConfig2);
        EXPECT_EQ(newVppGenerator.getConfig(), newConfig2);
        VppGenerator *newVppGenerator2 = new VppGenerator();
        EXPECT_NE(newVppGenerator2->getConfig(), vppConfig_);
        delete newVppGenerator2;
    }
}

TEST_F(VGeneratorTest, on_off) {
    {
        vdd_.stop();
        EXPECT_EQ(vdd_.start(), true);
        EXPECT_EQ(vdd_.isRunning(), true);
        EXPECT_EQ(vdd_.isOn(), false);
        EXPECT_EQ(vdd_.on(), true);
        EXPECT_EQ(vdd_.isOn(), true);
        vdd_.off();
        EXPECT_EQ(vdd_.isOn(), false);
        float vActual =
            calculate_((RAW_ADC_DATA[0] + RAW_ADC_DATA[1]) / 2,
                       vddConfig_.adcVref);
        EXPECT_EQ(vdd_.initCalibration(vActual), true);
        vdd_.adjust();
        EXPECT_EQ(vdd_.finishCalibration(vActual), true);
        vdd_.adjust();
        EXPECT_NEAR(vdd_.getV(), vActual, 0.2f);

        EXPECT_EQ(vdd_.isOn(), false);
        EXPECT_EQ(vdd_.on(), true);
        EXPECT_EQ(vdd_.isOn(), true);

        EXPECT_EQ(vdd_.isOnVpp(), false);
        EXPECT_EQ(vdd_.onVpp(), true);
        EXPECT_EQ(vdd_.isOnVpp(), true);
        EXPECT_EQ(vdd_.onVpp(false), true);
        EXPECT_EQ(vdd_.isOnVpp(), false);
    }
    {
        vpp_.stop();
        EXPECT_EQ(vpp_.start(), true);
        EXPECT_EQ(vpp_.isRunning(), true);
        EXPECT_EQ(vpp_.isOn(), false);
        EXPECT_EQ(vpp_.on(), true);
        EXPECT_EQ(vpp_.isOn(), true);
        vpp_.off();
        EXPECT_EQ(vpp_.isOn(), false);
        float vActual =
            calculate_((RAW_ADC_DATA[0] + RAW_ADC_DATA[1]) / 2,
                       vppConfig_.adcVref);
        EXPECT_EQ(vpp_.initCalibration(vActual), true);
        vpp_.adjust();
        EXPECT_EQ(vpp_.finishCalibration(vActual), true);
        vpp_.adjust();
        EXPECT_NEAR(vpp_.getV(), vActual, 0.2f);

        EXPECT_EQ(vpp_.isOn(), false);
        EXPECT_EQ(vpp_.on(), true);
        EXPECT_EQ(vpp_.isOn(), true);

        EXPECT_EQ(vpp_.isOnA9(), false);
        EXPECT_EQ(vpp_.onA9(), true);
        EXPECT_EQ(vpp_.isOnA9(), true);
        EXPECT_EQ(vpp_.onA9(false), true);
        EXPECT_EQ(vpp_.isOnA9(), false);

        EXPECT_EQ(vpp_.isOnA18(), false);
        EXPECT_EQ(vpp_.onA18(), true);
        EXPECT_EQ(vpp_.isOnA18(), true);
        EXPECT_EQ(vpp_.onA18(false), true);
        EXPECT_EQ(vpp_.isOnA18(), false);

        EXPECT_EQ(vpp_.isOnCE(), false);
        EXPECT_EQ(vpp_.onCE(), true);
        EXPECT_EQ(vpp_.isOnCE(), true);
        EXPECT_EQ(vpp_.onCE(false), true);
        EXPECT_EQ(vpp_.isOnCE(), false);

        EXPECT_EQ(vpp_.isOnOE(), false);
        EXPECT_EQ(vpp_.onOE(), true);
        EXPECT_EQ(vpp_.isOnOE(), true);
        EXPECT_EQ(vpp_.onOE(false), true);
        EXPECT_EQ(vpp_.isOnOE(), false);

        EXPECT_EQ(vpp_.isOnWE(), false);
        EXPECT_EQ(vpp_.onWE(), true);
        EXPECT_EQ(vpp_.isOnWE(), true);
        EXPECT_EQ(vpp_.onWE(false), true);
        EXPECT_EQ(vpp_.isOnWE(), false);
    }
}
