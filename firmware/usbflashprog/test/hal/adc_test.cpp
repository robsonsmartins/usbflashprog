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
 * @file test/hal/adc_test.cpp
 * @brief Implementation of Unit Test for Pico ADC Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "adc_test.hpp"

#include "mock/hardware/adc.h"

// ---------------------------------------------------------------------------

Adc AdcTest::adc_ = Adc();

// ---------------------------------------------------------------------------

float AdcTest::calculate_(uint16_t value, float vref) {
    float result = value * vref / (1 << 12);
    return result;
}

float AdcTest::mean_(const float* buf, size_t size) {
    float result = 0.0f;
    for (size_t i = 0; i < size; i++) {
        result += buf[i];
    }
    return result / size;
}

// ---------------------------------------------------------------------------

TEST_F(AdcTest, capture) {
    float buf[512];
    static uint16_t meanMockData =
        (RAW_ADC_DATA[0] + RAW_ADC_DATA[1]) / 2;
    EXPECT_NEAR(adc_.capture(0),
        AdcTest::calculate_(meanMockData), 0.2f);
    EXPECT_NEAR(adc_.capture(0, 1024),
        AdcTest::calculate_(meanMockData), 0.2f);
    EXPECT_NEAR(adc_.capture(0, buf, 512),
        AdcTest::calculate_(meanMockData), 0.2f);
    EXPECT_NEAR(AdcTest::mean_(buf, 512),
        AdcTest::calculate_(meanMockData), 0.2f);
    Adc newAdc = Adc(5.0f);
    EXPECT_NEAR(newAdc.capture(0),
        AdcTest::calculate_(meanMockData, 5.0f), 0.2f);
}
