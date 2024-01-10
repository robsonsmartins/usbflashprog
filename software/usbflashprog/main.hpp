// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2024) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @defgroup Software Software Project
 * @brief    Software project for USB EPROM/Flash Programmer.
 */
// ---------------------------------------------------------------------------
/**
 * @ingroup Software
 * @file main.hpp
 * @brief Header of the Main Routine.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef MAIN_HPP_
#define MAIN_HPP_

#include "config.hpp"

// ---------------------------------------------------------------------------

/**
 * @brief Loads application settings.
 * @return Application settings.
 */
TApplicationSettings loadSettings();

/**
 * @brief Initializes the logging feature.
 * @param level The log level (\<= 0 is disabled, 1 is FATAL, 2 is CRITICAL,
 *   etc).
 */
void initLogging(int level);

/**
 * @brief Loads the language strings.
 * @param language The language (en_US, or another).
 * @param app Pointer to the QApplication instance.
 */
void loadLanguage(const QString &language, QApplication *app);

// ---------------------------------------------------------------------------

#endif  // MAIN_HPP_
