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
 * @file test/hal/serial_test.hpp
 * @brief Header of Unit Test for Serial Communication Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_HAL_SERIAL_TEST_HPP_
#define TEST_HAL_SERIAL_TEST_HPP_

#include <gtest/gtest.h>
#include "hal/serial.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief Test class for Serial Communication Class.
 * @details The purpose of this class is to test the Serial
 *  Communication Class.
 * @nosubgrouping
 */
class SerialTest : public testing::Test {
 protected:
  /** @brief Constructor. */
  SerialTest() {}
  /** @brief Destructor. */
  ~SerialTest() override {}
  /** @brief Sets Up the test. */
  void SetUp() override {}
  /** @brief Teardown of the test. */
  void TearDown() override {}
  /* @brief Serial class object to test. */
  static Serial serial_;
};

#endif  // TEST_HAL_SERIAL_TEST_HPP_
