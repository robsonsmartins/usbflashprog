// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup Firmware
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

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Pico GPIO Class
 * @details The purpose of this class is to manipulate the GPIO pins on the
 *  board.
 * @nosubgrouping
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
    /**
     * @brief Returns if pin is pulled up.
     * @param pin The pin number.
     * @return True if pin is pulled up (pullup enabled).
     *  False otherwise.
     */
    bool isPulledUp(uint pin);
    /**
     * @brief Returns if pin is pulled down.
     * @param pin The pin number.
     * @return True if pin is pulled down (pulldown enabled).
     *  False otherwise.
     */
    bool isPulledDown(uint pin);

  private:
    /* @brief Set of initialized pins. */
    std::set<uint> initPins_;
    /*
     * @brief Initializes the pin.
     * @param pin The pin number.
     */
    void initPin_(uint pin);
};

#endif  // HAL_GPIO_HPP_
