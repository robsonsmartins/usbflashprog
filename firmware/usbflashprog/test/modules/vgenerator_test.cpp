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
#include "hal/multicore.hpp"

// ---------------------------------------------------------------------------

VGenerator VGeneratorTest::vGenerator_ = VGenerator();
VGenConfig VGeneratorTest::vGenConfig_ = VGenConfig();

// ---------------------------------------------------------------------------

VGeneratorTest::VGeneratorTest() {}

VGeneratorTest::~VGeneratorTest() {}

void VGeneratorTest::SetUp() {
    vGenConfig_.vdd.pwmPin = 0;
    vGenConfig_.vdd.adcChannel = 0;
    vGenConfig_.vdd.ctrlPin = 1;
    vGenConfig_.vdd.onVppPin = 2;
    vGenConfig_.vpp.pwmPin = 3;
    vGenConfig_.vpp.adcChannel = 1;
    vGenConfig_.vpp.ctrlPin = 4;
    vGenConfig_.vpp.vcSinPin = 5;
    vGenConfig_.vpp.vcClkPin = 6;
    vGenConfig_.vpp.vcClrPin = 7;
    vGenConfig_.vpp.vcRckPin = 8;
    vGenerator_.configure(vGenConfig_);
}

void VGeneratorTest::TearDown() {}

// ---------------------------------------------------------------------------

float VGeneratorTest::calculate_(uint16_t value, float vref) {
    float result = value * vref / (1 << 12);
    return result;
}

// ---------------------------------------------------------------------------

TEST_F(VGeneratorTest, start_stop) {
    EXPECT_EQ(vGenerator_.isRunning(), false);
    EXPECT_EQ(vGenerator_.vdd.isRunning(), false);
    EXPECT_EQ(vGenerator_.vpp.isRunning(), false);
    EXPECT_EQ(vGenerator_.start(), true);
    EXPECT_EQ(vGenerator_.isRunning(), true);
    EXPECT_EQ(vGenerator_.vdd.isRunning(), true);
    EXPECT_EQ(vGenerator_.vpp.isRunning(), true);
    vGenerator_.stop();
    EXPECT_EQ(vGenerator_.isRunning(), false);
    EXPECT_EQ(vGenerator_.vdd.isRunning(), false);
    EXPECT_EQ(vGenerator_.vpp.isRunning(), false);
}

TEST_F(VGeneratorTest, set_get_v_duty) {
    EXPECT_EQ(vGenerator_.isRunning(), false);

    EXPECT_EQ(vGenerator_.vdd.getVTarget(), 0.0f);
    EXPECT_EQ(vGenerator_.vdd.getDuty(), 0.0f);
    EXPECT_EQ(vGenerator_.vdd.getV(), 0.0f);
    EXPECT_EQ(vGenerator_.vpp.getVTarget(), 0.0f);
    EXPECT_EQ(vGenerator_.vpp.getDuty(), 0.0f);
    EXPECT_EQ(vGenerator_.vpp.getV(), 0.0f);

    EXPECT_EQ(vGenerator_.start(), true);
    EXPECT_EQ(vGenerator_.isRunning(), true);
    vGenerator_.vdd.on();
    vGenerator_.vdd.setV(5.0f);
    EXPECT_EQ(vGenerator_.vdd.getVTarget(), 5.0f);
    float vActual = calculate_((kRawAdcData[0] + kRawAdcData[1]) / 2,
                               vGenConfig_.vdd.adcVref);
    MultiCore::msleep(10);
    EXPECT_NEAR(vGenerator_.vdd.getV(), vActual, 0.2f);
    vGenerator_.vdd.setV(vActual * 1.06f);
    MultiCore::msleep(10);
    EXPECT_NEAR(vGenerator_.vdd.getV(), vActual * 1.06f, 0.2f);
    vGenerator_.vdd.setV(vActual * 0.94f);
    MultiCore::msleep(10);
    EXPECT_NEAR(vGenerator_.vdd.getV(), vActual * 0.94f, 0.2f);
    vGenerator_.vdd.setV(0.1f);
    MultiCore::msleep(10);
    EXPECT_NEAR(vGenerator_.vdd.getV(), vActual, 0.2f);

    vGenerator_.vpp.on();
    vGenerator_.vpp.setV(12.0f);
    EXPECT_EQ(vGenerator_.vpp.getVTarget(), 12.0f);
    vActual = calculate_((kRawAdcData[0] + kRawAdcData[1]) / 2,
                         vGenConfig_.vpp.adcVref);
    MultiCore::msleep(10);
    EXPECT_NEAR(vGenerator_.vpp.getV(), vActual, 0.2f);
    vGenerator_.vpp.setV(vActual * 1.06f);
    MultiCore::msleep(10);
    EXPECT_NEAR(vGenerator_.vpp.getV(), vActual * 1.06f, 0.2f);
    vGenerator_.vpp.setV(vActual * 0.94f);
    MultiCore::msleep(10);
    EXPECT_NEAR(vGenerator_.vpp.getV(), vActual * 0.94f, 0.2f);
    vGenerator_.vpp.setV(0.1f);
    MultiCore::msleep(10);
    EXPECT_NEAR(vGenerator_.vpp.getV(), vActual, 0.2f);
}

TEST_F(VGeneratorTest, constructor_config) {
    EXPECT_EQ(vGenerator_.getConfig(), vGenConfig_);
    VGenConfig newConfig;
    newConfig.vdd.ctrlPin = 20;
    newConfig.vpp.ctrlPin = 21;
    vGenerator_.configure(newConfig);
    EXPECT_NE(vGenerator_.getConfig(), vGenConfig_);
    EXPECT_EQ(vGenerator_.getConfig(), newConfig);
    VGenConfig newConfig2;
    newConfig2 = vGenConfig_;
    VGenerator newVGenerator(newConfig2);
    EXPECT_EQ(newVGenerator.getConfig(), newConfig2);
    VGenerator *newVGenerator2 = new VGenerator();
    EXPECT_NE(newVGenerator2->getConfig(), vGenConfig_);
    delete newVGenerator2;
}

TEST_F(VGeneratorTest, on_off) {
    EXPECT_EQ(vGenerator_.start(), true);
    EXPECT_EQ(vGenerator_.isRunning(), true);
    EXPECT_EQ(vGenerator_.vdd.isOn(), false);
    vGenerator_.vdd.on();
    EXPECT_EQ(vGenerator_.vdd.isOn(), true);
    vGenerator_.vdd.off();
    EXPECT_EQ(vGenerator_.vdd.isOn(), false);
    vGenerator_.vdd.toggle();
    EXPECT_EQ(vGenerator_.vdd.isOn(), true);
    vGenerator_.vdd.toggle();
    EXPECT_EQ(vGenerator_.vdd.isOn(), false);

    EXPECT_EQ(vGenerator_.vdd.isOnVpp(), false);
    vGenerator_.vdd.onVpp();
    EXPECT_EQ(vGenerator_.vdd.isOnVpp(), true);
    vGenerator_.vdd.onVpp(false);
    EXPECT_EQ(vGenerator_.vdd.isOnVpp(), false);

    EXPECT_EQ(vGenerator_.vpp.isOn(), false);
    vGenerator_.vpp.on();
    EXPECT_EQ(vGenerator_.vpp.isOn(), true);
    vGenerator_.vpp.off();
    EXPECT_EQ(vGenerator_.vpp.isOn(), false);
    vGenerator_.vpp.toggle();
    EXPECT_EQ(vGenerator_.vpp.isOn(), true);
    vGenerator_.vpp.toggle();
    EXPECT_EQ(vGenerator_.vpp.isOn(), false);

    EXPECT_EQ(vGenerator_.vpp.isOnA9(), false);
    vGenerator_.vpp.onA9();
    EXPECT_EQ(vGenerator_.vpp.isOnA9(), true);
    vGenerator_.vpp.onA9(false);
    EXPECT_EQ(vGenerator_.vpp.isOnA9(), false);

    EXPECT_EQ(vGenerator_.vpp.isOnA18(), false);
    vGenerator_.vpp.onA18();
    EXPECT_EQ(vGenerator_.vpp.isOnA18(), true);
    vGenerator_.vpp.onA18(false);
    EXPECT_EQ(vGenerator_.vpp.isOnA18(), false);

    EXPECT_EQ(vGenerator_.vpp.isOnCE(), false);
    vGenerator_.vpp.onCE();
    EXPECT_EQ(vGenerator_.vpp.isOnCE(), true);
    vGenerator_.vpp.onCE(false);
    EXPECT_EQ(vGenerator_.vpp.isOnCE(), false);

    EXPECT_EQ(vGenerator_.vpp.isOnOE(), false);
    vGenerator_.vpp.onOE();
    EXPECT_EQ(vGenerator_.vpp.isOnOE(), true);
    vGenerator_.vpp.onOE(false);
    EXPECT_EQ(vGenerator_.vpp.isOnOE(), false);

    EXPECT_EQ(vGenerator_.vpp.isOnWE(), false);
    vGenerator_.vpp.onWE();
    EXPECT_EQ(vGenerator_.vpp.isOnWE(), true);
    vGenerator_.vpp.onWE(false);
    EXPECT_EQ(vGenerator_.vpp.isOnWE(), false);
}

TEST_F(VGeneratorTest, calibration) {
    vGenerator_.start();
    float vActual = calculate_((kRawAdcData[0] + kRawAdcData[1]) / 2,
                               vGenConfig_.vdd.adcVref);
    vGenerator_.vdd.initCalibration(vActual);
    vGenerator_.vdd.saveCalibration(vActual);
    VGenerator newVGen;
    vActual = calculate_((kRawAdcData[0] + kRawAdcData[1]) / 2,
                         vGenConfig_.vpp.adcVref);
    vGenerator_.vpp.initCalibration(vActual);
    vGenerator_.vpp.saveCalibration(vActual);
    VGenerator newVGen2;
}
