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
#include <bitset>
#include <algorithm>
#include <cstring>
#include <cmath>

#include "hal/string.hpp"

// ---------------------------------------------------------------------------

std::string StringUtils::upper(const std::string &src) {
    std::string s = src;
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return s;
}

std::string StringUtils::lower(const std::string &src) {
    std::string s = src;
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s;
}

std::string StringUtils::trim(const std::string &src) {
    std::string s = src;
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char c) {
                return (!std::isspace(c));
            }));
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char c) { return (!std::isspace(c)); })
                .base(),
            s.end());
    return s;
}

StringUtils::TStringVector StringUtils::split(const std::string &src,
                                              char separator, bool trim) {
    TStringVector s;
    std::istringstream iss(src);
    std::string buf;
    while (getline(iss, buf, separator)) {
        if (!buf.empty()) {
            if (trim) {
                s.push_back(StringUtils::trim(buf));
            } else {
                s.push_back(buf);
            }
        }
    }
    return s;
}

int StringUtils::toInt(const std::string &src, uint base) {
    return std::strtol(src.c_str(), nullptr, base);
}

float StringUtils::toFloat(const std::string &src) {
    return std::strtof(src.c_str(), nullptr);
}

std::string StringUtils::fromInt(int src, uint base, uint digits, char fill) {
    std::ostringstream oss;
    std::string tmp;
    size_t pos;
    switch (base) {
        case 16:
            oss << std::setfill(fill) << std::setw(digits) << std::hex
                << std::uppercase << src;
            break;
        case 8:
            oss << std::setfill(fill) << std::setw(digits) << std::oct << src;
            break;
        case 2:
            tmp = std::bitset<32>(src).to_string();
            pos = tmp.find_first_not_of("0");
            if (pos != std::string::npos) {
                tmp = tmp.substr(pos);
            } else {
                tmp = "0";
            }
            for (size_t i = tmp.length(); i < digits; i++) {
                tmp = fill + tmp;
            }
            oss << tmp;
            break;
        case 10:
            oss << std::setfill(fill) << std::setw(digits) << std::dec << src;
            break;
        default:
            oss << src;
            break;
    }
    return oss.str();
}

std::string StringUtils::fromFloat(float src, uint precision) {
    std::ostringstream oss;
    oss.precision(precision);
    oss << std::fixed << src;
    return oss.str();
}
