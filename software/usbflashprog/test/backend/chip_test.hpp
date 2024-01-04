// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2024) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup UnitTests
 * @file test/backend/chip_test.hpp
 * @brief Header of Unit Test for Device (Chip) Classes.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_BACKEND_CHIP_TEST_HPP_
#define TEST_BACKEND_CHIP_TEST_HPP_

#include <gtest/gtest.h>

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief Test class for Device (Chip) Classes.
 * @details The purpose of this class is to test the Device (Chip) Classes.
 * @nosubgrouping
 */
class ChipTest : public testing::Test {
  protected:
    /** @brief Constructor. */
    ChipTest() {}
    /** @brief Destructor. */
    ~ChipTest() override {}
    /** @brief Sets Up the test. */
    void SetUp() override {}
    /** @brief Teardown of the test. */
    void TearDown() override {}
};

#endif  // TEST_BACKEND_CHIP_TEST_HPP_
