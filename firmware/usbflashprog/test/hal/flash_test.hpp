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
 * @file test/hal/flash_test.hpp
 * @brief Header of Unit Test for Pico Flash Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_HAL_FLASH_TEST_HPP_
#define TEST_HAL_FLASH_TEST_HPP_

#include <gtest/gtest.h>
#include "hal/flash.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief Test class for Pico Flash.
 * @details The purpose of this class is to test the Flash class.
 * @nosubgrouping
 */
class FlashTest : public testing::Test {
  protected:
    /** @brief Constructor. */
    FlashTest() {}
    /** @brief Destructor. */
    ~FlashTest() override {}
    /** @brief Sets Up the test. */
    void SetUp() override {}
    /** @brief Teardown of the test. */
    void TearDown() override {}
    /*
     * @brief Compares data and return if them are equal.
     * @param a One data.
     * @param b Another data.
     * @param len Size of data.
     * @return True if both data are equal, false otherwise.
     */
    bool compareData_(const uint8_t *a, const uint8_t *b, size_t len);
};

#endif  // TEST_HAL_FLASH_TEST_HPP_
