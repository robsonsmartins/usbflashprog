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
 * @file test/modules/vgenerator_test.hpp
 * @brief Header of Unit Test for Voltage Generator Classes.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_MODULES_VGENERATOR_TEST_HPP_
#define TEST_MODULES_VGENERATOR_TEST_HPP_

#include <gtest/gtest.h>
#include "modules/vgenerator.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief Test class for Voltage Generator.
 * @details The purpose of this class is to test the Voltage Generator
 *  Classes.
 * @nosubgrouping
 */
class VGeneratorTest : public testing::Test {
 protected:
  /** @brief Constructor. */
  VGeneratorTest();
  /** @brief Destructor. */
  ~VGeneratorTest() override;
  /** @brief Sets Up the test. */
  void SetUp() override;
  /** @brief Teardown of the test. */
  void TearDown() override;
  /* @brief VGenerator class object to test. */
  static VGenerator vGenerator_;
  /* @brief VGenConfig configuration used to test. */
  static VGenConfig vGenConfig_;
  /*
   * @brief Calculates the real value, based in VRef. 
   * @param value ADC raw value (12 bits).
   * @param vref Reference Voltage, in Volts (default = 3.3).
   * @return Real value, in Volts. 
   */
  static float calculate_(uint16_t value, float vref = 3.3f);
};

#endif  // TEST_MODULES_VGENERATOR_TEST_HPP_
