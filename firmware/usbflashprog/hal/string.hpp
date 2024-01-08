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
 * @file hal/string.hpp
 * @brief Header of the String Handling Helper Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef HAL_STRING_HPP_
#define HAL_STRING_HPP_

#include <string>
#include <vector>

#include "pico/stdlib.h"

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief String Handling Helper Class
 * @details The purpose of this class is to handle strings.
 * @nosubgrouping
 */
class StringUtils {
  public:
    /** @brief Vector of strings type. */
    typedef std::vector<std::string> TStringVector;
    /**
     * @brief Converts a string to uppercase.
     * @param src Source string.
     * @return String in uppercase.
     */
    static std::string upper(const std::string &src);
    /**
     * @brief Converts a string to lowercase.
     * @param src Source string.
     * @return String in lowercase.
     */
    static std::string lower(const std::string &src);
    /**
     * @brief Removes blank characters from left/right of the string.
     * @param src Source string.
     * @return Cleaned string.
     */
    static std::string trim(const std::string &src);
    /**
     * @brief Splits a string into a vector, from a separator.
     * @param src Source string.
     * @param separator Separator character.
     * @param trim If true, cleans each token before add into vector.
     *   @see trim(const std::string&). Default is false.
     * @return Vector with string tokens.
     */
    static TStringVector split(const std::string &src, char separator,
                               bool trim = false);
    /**
     * @brief Converts a string to an integer value.
     * @param src Source string.
     * @param base Numerical base. Default is 10 (decimal).
     * @return String as integer value, or zero if fail.
     */
    static int toInt(const std::string &src, uint base = 10);
    /**
     * @brief Converts a string to a float point value.
     * @param src Source string.
     * @return String as float point value, or zero if fail.
     */
    static float toFloat(const std::string &src);
    /**
     * @brief Converts an integer value to a string.
     * @param src Integer value.
     * @param base Numerical base. Default is 10 (decimal).
     * @param digits Number of min digits to format
     *  (left pad with <i>fill</i>). Default is zero.
     * @param fill Fill character. Default is space.
     * @return Numerical value as string.
     */
    static std::string fromInt(int src, uint base = 10, uint digits = 0,
                               char fill = ' ');
    /**
     * @brief Converts a float point value to a string.
     * @param src Float point value.
     * @param precision Number of decimals (precision). Default is 3.
     * @return Numerical value as string.
     */
    static std::string fromFloat(float src, uint precision = 3);
};

#endif  // HAL_STRING_HPP_
