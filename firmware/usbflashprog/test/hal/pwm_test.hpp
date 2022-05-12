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
 * @file test/hal/pwm_test.hpp
 * @brief Header of Unit Test for Pico PWM Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_HAL_PWM_TEST_HPP_
#define TEST_HAL_PWM_TEST_HPP_

#include <gtest/gtest.h>
#include "hal/pwm.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief Test class for Pico PWM.
 * @details The purpose of this class is to test the Pwm class.
 * @nosubgrouping
 */
class PwmTest : public testing::Test {
 protected:
  /** @brief Constructor. */
  PwmTest() {}
  /** @brief Destructor. */
  ~PwmTest() override {}
  /** @brief Sets Up the test. */
  void SetUp() override {}
  /** @brief Teardown of the test. */
  void TearDown() override {}
  /* @brief Pwm class object to test. */
  static Pwm pwm_;
};

#endif  // TEST_HAL_PWM_TEST_HPP_
