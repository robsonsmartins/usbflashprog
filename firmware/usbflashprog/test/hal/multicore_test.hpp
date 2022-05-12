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
 * @file test/hal/multicore_test.hpp
 * @brief Header of Unit Test for Pico Multi Core Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_HAL_MULTICORE_TEST_HPP_
#define TEST_HAL_MULTICORE_TEST_HPP_

#include <gtest/gtest.h>
#include "hal/multicore.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief Test class for Pico Multi Core.
 * @details The purpose of this class is to test the MultiCore class.
 * @nosubgrouping
 */
class MultiCoreTest : public testing::Test {
 protected:
  /** @brief Constructor. */
  MultiCoreTest() {}
  /** @brief Destructor. */
  ~MultiCoreTest() override {}
  /** @brief Sets Up the test. */
  void SetUp() override {}
  /** @brief Teardown of the test. */
  void TearDown() override {}
  /* @brief MultiCore class object to test. */
  static MultiCore multicore_;
};

#endif  // TEST_HAL_MULTICORE_TEST_HPP_
