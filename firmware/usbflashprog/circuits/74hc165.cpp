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
 * @file circuits/74hc165.cpp
 * @brief Implementation of the 74HC165 Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "circuits/74hc165.hpp"

// ---------------------------------------------------------------------------

HC165::HC165() : ce_(false) {
    configure();
}

HC165::HC165(uint plPin, uint clkPin, uint cePin, uint q7Pin, uint nq7Pin,
             uint pulseTime)
    : ce_(false) {
    configure(plPin, clkPin, cePin, q7Pin, nq7Pin, pulseTime);
}

void HC165::configure(uint plPin, uint clkPin, uint cePin, uint q7Pin,
                      uint nq7Pin, uint pulseTime) {
    plPin_ = plPin;
    clkPin_ = clkPin;
    cePin_ = cePin;
    q7Pin_ = q7Pin;
    nq7Pin_ = nq7Pin;
    pulseTime_ = pulseTime;
}

void HC165::chipEnable(bool value) {
    if (cePin_ != 0xFF) {
        gpio_.setPin(cePin_, !value);
    }
    ce_ = value;
}

void HC165::chipDisable(void) {
    if (cePin_ != 0xFF) {
        gpio_.setPin(cePin_);
    }
    ce_ = false;
}

const bool HC165::getCE(void) const {
    return ce_;
}

void HC165::load() {
    if (plPin_ != 0xFF) {
        gpio_.resetPin(plPin_);
        sleep_us(pulseTime_);
        gpio_.setPin(plPin_);
    }
}

bool HC165::getBit(uint index) {
    if (clkPin_ == 0xFF || (q7Pin_ == 0xFF && nq7Pin_ == 0xFF)) {
        return false;
    }
    for (uint i = 0; i < index; i++) {
        gpio_.setPin(clkPin_);
        sleep_us(pulseTime_);
        gpio_.resetPin(clkPin_);
        sleep_us(pulseTime_);
    }
    if (q7Pin_ != 0xFF) {
        return (gpio_.getPin(q7Pin_));
    } else {
        return (!gpio_.getPin(nq7Pin_));
    }
}

uint8_t HC165::readByte(bool reverse) {
    uint8_t buffer = 0;
    if (readData(&buffer, 1, reverse) != 1) {
        return 0;
    }
    return buffer;
}

uint16_t HC165::readWord(bool reverse) {
    uint8_t buffer[2];
    if (readData(buffer, 2, reverse) != 2) {
        return 0;
    }
    uint16_t result = buffer[1];
    result <<= 8;
    result |= buffer[0];
    return result;
}

uint32_t HC165::readDWord(bool reverse) {
    uint8_t buffer[4];
    if (readData(buffer, 4, reverse) != 4) {
        return 0;
    }
    uint32_t result = buffer[3];
    result <<= 8;
    result |= buffer[2];
    result <<= 8;
    result |= buffer[1];
    result <<= 8;
    result |= buffer[0];
    return result;
}

uint HC165::readData(uint8_t* buffer, uint size, bool reverse) {
    if (!buffer || !size) {
        return 0;
    }
    if (clkPin_ == 0xFF || (q7Pin_ == 0xFF && nq7Pin_ == 0xFF)) {
        return 0;
    }
    TData bits;
    for (uint i = 0; i < size * 8; i++) {
        if (q7Pin_ != 0xFF) {
            bits.push_back(gpio_.getPin(q7Pin_));
        } else {
            bits.push_back(!gpio_.getPin(nq7Pin_));
        }
        gpio_.setPin(clkPin_);
        sleep_us(pulseTime_);
        gpio_.resetPin(clkPin_);
        sleep_us(pulseTime_);
    }
    std::size_t start = reverse ? 0 : bits.size() - 1;
    std::size_t end = reverse ? bits.size() - 1 : 0;
    std::size_t nbit = start;
    for (uint nbyte = 0; nbyte < size; nbyte++) {
        buffer[nbyte] = 0;
        for (uint b = 0; b < 8; b++) {
            if (bits[nbit]) {
                buffer[nbyte] |= (1 << b);
            }
            if (nbit == end) {
                break;
            }
            if (reverse) {
                nbit++;
            } else {
                nbit--;
            }
        }
        if (nbit == end) {
            break;
        }
    }
    return size;
}
