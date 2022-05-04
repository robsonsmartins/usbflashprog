// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/** 
 * @file test/hal/gpio_test.hpp
 * @brief Header of Unit Test for Pico GPIO Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_HAL_GPIO_TEST_HPP_
#define TEST_HAL_GPIO_TEST_HPP_

#include <gtest/gtest.h>
#include "hal/gpio.hpp"

/**
 * @brief Test class for Pico GPIO.
 * @details The purpose of this class is to test the Gpio class.
 */
class GpioTest : public testing::Test {
 protected:
  /** @brief Constructor. */
  GpioTest() {}
  /** @brief Destructor. */
  ~GpioTest() override {}
  /** @brief Sets Up the test. */
  void SetUp() override {}
  /** @brief Teardown of the test. */
  void TearDown() override {}
  /** @brief Gpio class object to test. */
  static Gpio gpio_;
};

#endif  // TEST_HAL_GPIO_TEST_HPP_
