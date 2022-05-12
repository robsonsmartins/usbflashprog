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
 * @file test/hal/adc_test.hpp
 * @brief Header of Unit Test for Pico ADC Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_HAL_ADC_TEST_HPP_
#define TEST_HAL_ADC_TEST_HPP_

#include <gtest/gtest.h>
#include "hal/adc.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief Test class for Pico ADC.
 * @details The purpose of this class is to test the Adc class.
 * @nosubgrouping
 */
class AdcTest : public testing::Test {
 protected:
  /** @brief Constructor. */
  AdcTest() {}
  /** @brief Destructor. */
  ~AdcTest() override {}
  /** @brief Sets Up the test. */
  void SetUp() override {}
  /** @brief Teardown of the test. */
  void TearDown() override {}
  /* @brief Adc class object to test. */
  static Adc adc_;
  /*
   * @brief Calculates the real value, based in VRef. 
   * @param value ADC raw value (12 bits).
   * @param vref Reference Voltage, in Volts (default = 3.3).
   * @return Real value, in Volts. 
   */
  static float calculate_(uint16_t value, float vref = 3.3f);
  /*
   * @brief Calculates the mean of samples into buffer.
   * @param buf Pointer to buffer of samples.
   * @param size Size of buffer.
   * @return Mean of all samples, in Volts. 
   */
  static float mean_(const float* buf, size_t size);
};

#endif  // TEST_HAL_ADC_TEST_HPP_
