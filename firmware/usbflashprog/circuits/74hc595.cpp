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
 * @file circuits/74hc595.cpp
 * @brief Implementation of the 74HC595 Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "circuits/74hc595.hpp"

// ---------------------------------------------------------------------------

HC595::HC595() : oe_(false) {
    configure();
}

HC595::HC595(uint sinPin, uint clkPin, uint clrPin, uint rckPin, uint oePin,
             uint pulseTime)
    : oe_(false) {
    configure(sinPin, clkPin, clrPin, rckPin, oePin, pulseTime);
}

void HC595::configure(uint sinPin, uint clkPin, uint clrPin, uint rckPin,
                      uint oePin, uint pulseTime) {
    sinPin_ = sinPin;
    clkPin_ = clkPin;
    clrPin_ = clrPin;
    rckPin_ = rckPin;
    oePin_ = oePin;
    pulseTime_ = pulseTime;
}

void HC595::clear() {
    if (clrPin_ != 0xFF) {
        gpio_.resetPin(clrPin_);
        sleep_us(pulseTime_);
        gpio_.setPin(clrPin_);
    }
    if (rckPin_ != 0xFF) {
        gpio_.setPin(rckPin_);
        sleep_us(pulseTime_);
        gpio_.resetPin(rckPin_);
    }
    buffer_.clear();
}

void HC595::outputEnable(bool value) {
    if (oePin_ != 0xFF) {
        gpio_.setPin(oePin_, !value);
    }
    oe_ = value;
}

void HC595::outputDisable() {
    outputEnable(false);
}

void HC595::writeByte(uint8_t value) {
    clear();
    if (!value) {
        return;
    }
    writeData(&value, 1);
}

void HC595::writeWord(uint16_t value) {
    clear();
    if (!value) {
        return;
    }
    if (value <= 0xFF) {
        writeByte(value & 0xFF);
        return;
    }
    uint8_t buffer[2];
    buffer[0] = value & 0xFF;
    buffer[1] = (value & 0xFF00) >> 8;
    writeData(buffer, 2);
}

void HC595::writeDWord(uint32_t value) {
    clear();
    if (!value) {
        return;
    }
    if (value <= 0xFF) {
        writeByte(value & 0xFF);
        return;
    } else if (value <= 0xFFFF) {
        writeWord(value & 0xFFFF);
        return;
    }
    uint8_t buffer[4];
    buffer[0] = value & 0xFF;
    buffer[1] = (value & 0xFF00) >> 8;
    buffer[2] = (value & 0xFF0000) >> 16;
    buffer[3] = (value & 0xFF000000) >> 24;
    writeData(buffer, (value <= 0xFFFFFF) ? 3 : 4);
}

void HC595::writeData(const uint8_t* buffer, uint size) {
    if (!size || !buffer) {
        return;
    }
    if (buffer_.size() < size) {
        buffer_.resize(size);
    }
    if (rckPin_ != 0xFF) {
        gpio_.resetPin(rckPin_);
    }
    buffer += size - 1;
    for (uint8_t* pData = buffer_.data() + size - 1; size != 0;
         size--, pData--, buffer--) {
        *pData = *buffer;
        if (sinPin_ != 0xFF && clkPin_ != 0xFF) {
            gpio_.resetPin(clkPin_);
            for (int bit = 7; bit >= 0; bit--) {
                gpio_.setPin(sinPin_, (*pData) & (1 << bit));
                sleep_us(pulseTime_);
                gpio_.setPin(clkPin_);
                sleep_us(pulseTime_);
                gpio_.resetPin(clkPin_);
            }
        }
        if (rckPin_ != 0xFF) {
            gpio_.setPin(rckPin_);
            sleep_us(pulseTime_);
            gpio_.resetPin(rckPin_);
        }
    }
}

void HC595::setBit(uint bit, bool value) {
    uint index = bit / 8;
    if (index + 1 > buffer_.size()) {
        buffer_.resize(index + 1);
    }
    uint8_t data = buffer_[index];
    uint8_t mask = 0x01 << (bit - (index * 8));
    if (value) {
        data |= mask;
    } else {
        data &= ~mask;
    }
    buffer_[index] = data;
    writeData(buffer_.data(), buffer_.size());
}

void HC595::resetBit(uint bit) {
    setBit(bit, false);
}

void HC595::toggleBit(uint bit) {
    uint index = bit / 8;
    if (index + 1 > buffer_.size()) {
        buffer_.resize(index + 1);
    }
    uint8_t data = buffer_[index];
    uint8_t mask = 0x01 << (bit - (index * 8));
    data ^= mask;
    buffer_[index] = data;
    writeData(buffer_.data(), buffer_.size());
}

const HC595::TData& HC595::getData(void) const {
    return buffer_;
}

const bool HC595::getBit(uint bit) const {
    uint index = bit / 8;
    uint8_t data = (index < buffer_.size()) ? buffer_[index] : 0;
    uint8_t mask = 0x01 << (bit - (index * 8));
    return (data & mask);
}

const bool HC595::getOE(void) const {
    return oe_;
}
