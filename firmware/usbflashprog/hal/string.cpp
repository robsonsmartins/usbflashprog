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
 * @file hal/string.cpp
 * @brief Implementation of the String Handling Helper Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cmath>

#include "hal/string.hpp"

// ---------------------------------------------------------------------------

StringUtils::StringUtils() {}

std::string StringUtils::upper(const std::string &src) {
    std::string s = src;
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::toupper(c); });
    return s;
}

std::string StringUtils::trim(const std::string &src) {
    std::string s = src;
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
    return s;
}

StringUtils::TStringVector StringUtils::split(const std::string &src,
                                              char separator) {
    TStringVector s;
    std::istringstream iss(src);
    std::string buf;
    while (getline(iss, buf, separator)) {
        s.push_back(buf);
    }
    return s;
}

int StringUtils::toInt(const std::string &src, int base) {
    return std::strtol(src.c_str(), nullptr, base);
}

float StringUtils::toFloat(const std::string &src) {
    return std::strtof(src.c_str(), nullptr);
}

std::string StringUtils::fromInt(int src, int base) {
    std::ostringstream oss;
    switch (base) {
        case 16:
            oss << std::setfill('0') << std::setw(2)
                << std::hex << std::uppercase;
            break;
        case 8:
            oss << std::oct;
            break;
        case 10:
            oss << std::dec;
            break;
        default:
            break;
    }
    oss << src;
    return oss.str();
}

std::string StringUtils::fromFloat(float src) {
    std::ostringstream oss;
    oss << src;
    return oss.str();
}
