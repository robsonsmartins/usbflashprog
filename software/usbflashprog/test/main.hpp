// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2024) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @defgroup UnitTests Unit Tests
 * @brief    Unit Tests for USB EPROM/Flash Programmer software project.
 */
// ---------------------------------------------------------------------------
/**
 * @ingroup UnitTests
 * @file test/main.hpp
 * @brief Header of the Unit Tests Main Routine.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_MAIN_HPP_
#define TEST_MAIN_HPP_

// ---------------------------------------------------------------------------

#include <cstdint>

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief Define the log level for tests.
 * @details The log levels: 0 (default) is disabled; 1 is fatal; 2 is critical;
 *   3 is warning; 4 is info; 5 (or more) is debug.
 */
constexpr uint8_t kTestLogLevel = 0;

// ---------------------------------------------------------------------------

#endif  // TEST_MAIN_HPP_
