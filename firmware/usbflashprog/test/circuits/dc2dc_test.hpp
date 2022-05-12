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
 * @file test/circuits/dc2dc_test.hpp
 * @brief Header of Unit Test for DC To DC Converter Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_CIRCUITS_DC2DC_TEST_HPP_
#define TEST_CIRCUITS_DC2DC_TEST_HPP_

#include <gtest/gtest.h>
#include "circuits/dc2dc.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief Test class for DC To DC Converter.
 * @details The purpose of this class is to test the DC To DC Converter class.
 * @nosubgrouping
 */
class Dc2DcTest : public testing::Test {
 protected:
  /** @brief Constructor. */
  Dc2DcTest();
  /** @brief Destructor. */
  ~Dc2DcTest() override;
  /** @brief Sets Up the test. */
  void SetUp() override;
  /** @brief Teardown of the test. */
  void TearDown() override;
  /* @brief Dc2Dc class object to test. */
  static Dc2Dc dc2dc_;
  /* @brief Dc2DcConfig configuration used to test. */
  static Dc2DcConfig config_;
  /*
   * @brief Calculates the real value, based in VRef. 
   * @param value ADC raw value (12 bits).
   * @param vref Reference Voltage, in Volts (default = 3.3).
   * @return Real value, in Volts. 
   */
  static float calculate_(uint16_t value, float vref = 3.3f);
};

#endif  // TEST_CIRCUITS_DC2DC_TEST_HPP_
