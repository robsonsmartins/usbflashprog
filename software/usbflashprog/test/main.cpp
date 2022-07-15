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
 * @brief    Unit Tests for USB EPROM/Flash Programmer software project.
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
#include <QCoreApplication>
#include <QTimer>

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief Main routine for tests.
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 * @return Error code (zero if success).
 */
int main(int argc, char **argv) {
  QCoreApplication app{argc, argv};
  QTimer::singleShot(0, [&]() {
    ::testing::InitGoogleTest(&argc, argv);
    auto testResult = RUN_ALL_TESTS();
    app.exit(testResult);
  });
  return app.exec();
}
