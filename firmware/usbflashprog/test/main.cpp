// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @defgroup UnitTests Unit Tests
 * @brief    Unit Tests for USB EPROM/Flash Programmer firmware project.
 */
// ---------------------------------------------------------------------------
/**
 * @ingroup UnitTests
 * @file test/main.cpp
 * @brief Implementation of the Unit Tests Main Routine.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <gtest/gtest.h>

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief Main routine for tests.
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 * @return Error code (zero if success).
 */
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
