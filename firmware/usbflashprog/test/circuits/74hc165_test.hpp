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
 * @file test/circuits/74hc165_test.hpp
 * @brief Header of Unit Test for 74xx165 Shift Register Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_CIRCUITS_74HC165_TEST_HPP_
#define TEST_CIRCUITS_74HC165_TEST_HPP_

#include <gtest/gtest.h>
#include "circuits/74hc165.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief Test class for 74xx165 Shift Register.
 * @details The purpose of this class is to test the 74xx165
 *  Shift Register class.
 * @nosubgrouping
 */
class HC165Test : public testing::Test {
  protected:
    /** @brief Constructor. */
    HC165Test();
    /** @brief Destructor. */
    ~HC165Test() override;
    /** @brief Sets Up the test. */
    void SetUp() override;
    /** @brief Teardown of the test. */
    void TearDown() override;
    /* @brief HC165 class object to test. */
    static HC165 hc165_;
};

#endif  // TEST_CIRCUITS_74HC165_TEST_HPP_
