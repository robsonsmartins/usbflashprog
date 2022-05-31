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
 * @file test/hal/string_test.hpp
 * @brief Header of Unit Test for String Handling Helper Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_HAL_STRING_TEST_HPP_
#define TEST_HAL_STRING_TEST_HPP_

#include <gtest/gtest.h>
#include "hal/string.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief Test class for String Handling Helper Class.
 * @details The purpose of this class is to test the String Handling helper
 *  class.
 * @nosubgrouping
 */
class StringTest : public testing::Test {
 protected:
  /** @brief Constructor. */
  StringTest() {}
  /** @brief Destructor. */
  ~StringTest() override {}
  /** @brief Sets Up the test. */
  void SetUp() override {}
  /** @brief Teardown of the test. */
  void TearDown() override {}
};

#endif  // TEST_HAL_STRING_TEST_HPP_
