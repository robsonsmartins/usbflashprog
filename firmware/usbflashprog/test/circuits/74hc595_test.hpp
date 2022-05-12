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
 * @file test/circuits/74hc595_test.hpp
 * @brief Header of Unit Test for 74xx595 Shift Register Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_CIRCUITS_74HC595_TEST_HPP_
#define TEST_CIRCUITS_74HC595_TEST_HPP_

#include <gtest/gtest.h>
#include "circuits/74hc595.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief Test class for 74xx595 Shift Register.
 * @details The purpose of this class is to test the 74xx595
 *  Shift Register class.
 * @nosubgrouping
 */
class HC595Test : public testing::Test {
 protected:
  /** @brief Constructor. */
  HC595Test();
  /** @brief Destructor. */
  ~HC595Test() override;
  /** @brief Sets Up the test. */
  void SetUp() override;
  /** @brief Teardown of the test. */
  void TearDown() override;
  /*
   * @brief Compares data and return if them are equal.
   * @param a One data.
   * @param b Another data.
   * @return True if both data are equal, false otherwise.
   */
  bool compareData_(const HC595::TData& a,
                   const HC595::TData& b);
  /* @brief HC595 class object to test. */
  static HC595 hc595_;
};

#endif  // TEST_CIRCUITS_74HC595_TEST_HPP_
