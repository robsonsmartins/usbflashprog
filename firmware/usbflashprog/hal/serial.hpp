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
 * @file hal/serial.hpp
 * @brief Header of the Pico Serial Communication Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef HAL_SERIAL_HPP_
#define HAL_SERIAL_HPP_

#include <string>
#include <iostream>

#include "pico/stdlib.h"

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Pico Serial Communication Class
 * @details The purpose of this class is to handle the serial communication
 *  (via UART or via USB-CDC).
 * @nosubgrouping
 */
class Serial {
 public:
    /** @brief Constructor. */
    Serial();
    /**
     * @brief Gets a single character.
     * @param us Timeout, in microseconds. Default is zero (returns immediately).
     * @return A single char, or PICO_ERROR_TIMEOUT if timeout is reached.
     */
    int getChar(uint32_t us = 0);
    /**
     * @brief Gets a buffer.
     * @param buf Pointer to buffer that receives the data.
     * @param len Size of buffer, in bytes.
     * @param us Timeout, in microseconds. Default is zero (returns immediately).
     * @return Number of bytes read, or zero if timeout is reached.
     */
    size_t getBuf(void *buf, size_t len, uint32_t us);
    /**
     * @brief Gets a string (blocks until enter/0x0A).
     * @return The string.
     */
    std::string getStr();
    /**
     * @brief Gets an integer value (blocks until enter/0x0A).
     * @param base Numerical base. Default is 10 (decimal).
     * @return The numeric value.
     */
    int getInt(int base = 10);
    /**
     * @brief Gets a float point value (blocks until enter/0x0A).
     * @return The numeric value.
     */
    float getFloat();
    /**
     * @brief Puts a single character.
     * @param c Character to write.
     * @param flush If true, flushes the output. Default is false.
     */
    void putChar(char c, bool flush = false);
    /**
     * @brief Puts a buffer of data.
     * @param src Pointer to buffer to write.
     * @param len Size of buffer, in bytes.
     * @param flush If true, flushes the output. Default is false.
     */
    void putBuf(const void *src, size_t len, bool flush = false);
    /**
     * @brief Puts a string.
     * @param src String to write.
     * @param flush If true, flushes the output. Default is false.
     */
    void putStr(const std::string &src, bool flush = false);
    /**
     * @brief Puts an integer value.
     * @param src Numeric value to write.
     * @param base Numerical base. Default is 10 (decimal).
     * @param flush If true, flushes the output. Default is false.
     */
    void putInt(int src, int base = 10, bool flush = false);
    /**
     * @brief Puts a float point value.
     * @param src Numeric value to write.
     * @param flush If true, flushes the output. Default is false.
     */
    void putFloat(float src, bool flush = false);
    /**
     * @brief Returns the output stream object.
     * @return Reference to the output stream object.
     */
    std::ostream& out();
    /**
     * @brief Returns the input stream object.
     * @return Reference to the input stream object.
     */
    std::istream& in();
};

#endif  // HAL_SERIAL_HPP_
