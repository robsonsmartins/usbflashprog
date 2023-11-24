// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2023) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/** 
 * @ingroup UnitTests
 * @file test/backend/runner_test.hpp
 * @brief Header of Unit Test for Runner Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_BACKEND_RUNNER_TEST_HPP_
#define TEST_BACKEND_RUNNER_TEST_HPP_

#include <gtest/gtest.h>

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief Test class for Runner Class.
 * @details The purpose of this class is to test the Runner Class.
 * @nosubgrouping
 */
class RunnerTest : public testing::Test {
 protected:
  /** @brief Constructor. */
  RunnerTest() {}
  /** @brief Destructor. */
  ~RunnerTest() override {}
  /** @brief Sets Up the test. */
  void SetUp() override {}
  /** @brief Teardown of the test. */
  void TearDown() override {}
};

#endif  // TEST_BACKEND_RUNNER_TEST_HPP_
