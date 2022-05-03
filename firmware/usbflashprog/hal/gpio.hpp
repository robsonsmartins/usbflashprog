// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/** 
 * @file hal/gpio.hpp
 * @brief Header of the Pico GPIO Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef HAL_GPIO_HPP_
#define HAL_GPIO_HPP_

#include <set>

#include "pico/stdlib.h"

/**
 * @brief Pico GPIO Class
 * @details The purpose of this class is to manipulate the GPIO pins on the 
 *  board.
 */
class Gpio {
 public:
    /** @brief Constructor. */
    Gpio();
    /**
     * @brief Sets the pin value.
     * @param pin The pin number.
     * @param value Value to set (default = true).
     */
    void setPin(uint pin, bool value = true);
    /**
     * @brief Resets the pin value.
     * @param pin The pin number.
     */
    void resetPin(uint pin);
    /**
     * @brief Toggles the pin value.
     * @param pin The pin number.
     */
    void togglePin(uint pin);
    /**
     * @brief Gets the pin value.
     * @param pin The pin number.
     * @return The pin value. 
     */
    bool getPin(uint pin);
    /**
     * @brief Enables (or disables) the pullup of the pin.
     * @param pin The pin number.
     * @param enable True to enable pullup (default), or false to disable. 
     */
    void pullUp(uint pin, bool enable = true);
    /**
     * @brief Enables (or disables) the pulldown of the pin.
     * @param pin The pin number.
     * @param enable True to enable pulldown (default), or false to disable. 
     */
    void pullDown(uint pin, bool enable = true);
    /**
     * @brief Disables both the pullup and the pulldown of the pin.
     * @param pin The pin number.
     */
    void disablePulls(uint pin);

 private:
    /** @brief Set of initialized pins. */
    std::set<uint> _initPins;
    /**
     * @brief Initializes the pin.
     * @param pin The pin number.
     */
    void _initPin(uint pin);
};

#endif  // HAL_GPIO_HPP_
