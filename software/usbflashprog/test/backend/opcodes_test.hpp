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
 * @file test/backend/opcodes_test.hpp
 * @brief Header of Unit Test for OpCode Handling Classes.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_BACKEND_OPCODES_TEST_HPP_
#define TEST_BACKEND_OPCODES_TEST_HPP_

#include <gtest/gtest.h>

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief Test class for OpCode Handling Classes.
 * @details The purpose of this class is to test the OpCode Handling Classes.
 * @nosubgrouping
 */
class OpCodeTest : public testing::Test {
  protected:
    /** @brief Constructor. */
    OpCodeTest() {}
    /** @brief Destructor. */
    ~OpCodeTest() override {}
    /** @brief Sets Up the test. */
    void SetUp() override {}
    /** @brief Teardown of the test. */
    void TearDown() override {}
};

#endif  // TEST_BACKEND_OPCODES_TEST_HPP_
