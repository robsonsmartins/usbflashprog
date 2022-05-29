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
 * @file hal/serial.cpp
 * @brief Implementation of the Pico Serial Communication Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <cstring>

#include "hal/serial.hpp"
#include "hal/string.hpp"

// ---------------------------------------------------------------------------

Serial::Serial() {
    stdio_init_all();
}

int Serial::getChar(uint32_t us) {
    return getchar_timeout_us(us);
}

size_t Serial::getBuf(void *buf, size_t len, uint32_t us) {
    if (!buf || !len) { return 0; }
    size_t rd = 0;
    char *p = reinterpret_cast<char*>(buf);
    int c;
    do {
        c = getchar_timeout_us(us);
        if (c != PICO_ERROR_TIMEOUT) {
            *p = c;
            p++;
            rd++;
        } else { break; }
    } while (rd < len);
    return rd;
}

std::string Serial::getStr() {
    std::string result;
    std::cin >> result;
    return result;
}

int Serial::getInt(int base) {
    return StringUtils::toInt(getStr(), base);
}

float Serial::getFloat() {
    return StringUtils::toFloat(getStr());
}

void Serial::putChar(char c, bool flush) {
    putchar_raw(c);
    if (flush) { std::cout.flush(); }
}

void Serial::putBuf(const void *src, size_t len, bool flush) {
    if (!src || !len) { return; }
    const char *p = reinterpret_cast<const char*>(src);
    for (size_t i = 0; i < len; i++) {
        putchar_raw(p[i]);
    }
    if (flush) { std::cout.flush(); }
}

void Serial::putStr(const std::string &src, bool flush) {
    if (src.empty()) { return; }
    std::cout << src;
    if (flush) { std::cout.flush(); }
}

void Serial::putInt(int src, int base, bool flush) {
    std::cout << StringUtils::fromInt(src, base);
    if (flush) { std::cout.flush(); }
}

void Serial::putFloat(float src, bool flush) {
    std::cout << src;
    if (flush) { std::cout.flush(); }
}

std::ostream& Serial::out() {
    return std::cout;
}

std::istream& Serial::in() {
    return std::cin;
}
