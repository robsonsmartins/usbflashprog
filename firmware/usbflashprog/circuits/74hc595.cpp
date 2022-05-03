// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @file circuits/74hc595.cpp
 * @brief Implementation of the 74HC595 Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "circuits/74hc595.hpp"

HC595::HC595(): _oe(false) {
    configure();
}

HC595::HC595(uint sinPin, uint clkPin, uint clrPin,
             uint rckPin, uint oePin, uint pulseTime): _oe(false) {
    configure(sinPin, clkPin, clrPin, rckPin, oePin, pulseTime);
}

void HC595::configure(uint sinPin, uint clkPin, uint clrPin,
                      uint rckPin, uint oePin, uint pulseTime) {
    _sinPin    =    sinPin;
    _clkPin    =    clkPin;
    _clrPin    =    clrPin;
    _rckPin    =    rckPin;
    _oePin     =     oePin;
    _pulseTime = pulseTime;
}

void HC595::clear() {
    if (_clrPin != 0xFF) {
        _gpio.resetPin(_clrPin);
        sleep_us(_pulseTime);
        _gpio.setPin(_clrPin);
    }
    for (auto& data : _buffer) { data = 0; }
}

void HC595::outputEnable(bool value) {
    if (_oePin != 0xFF) {
        _gpio.setPin(_oePin, !value);
    }
    _oe = value;
}

void HC595::outputDisable() {
    outputEnable(false);
}

void HC595::writeByte(uint8_t value) {
    writeData(&value, 1);
}

void HC595::writeWord(uint16_t value) {
    uint8_t buffer[2];
    buffer[0] = value & 0xFF;
    buffer[1] = (value & 0xFF00) >> 8;
    writeData(buffer, 2);
}

void HC595::writeDWord(uint32_t value) {
    uint8_t buffer[4];
    buffer[0] = value & 0xFF;
    buffer[1] = (value & 0xFF00) >> 8;
    buffer[2] = (value & 0xFF0000) >> 16;
    buffer[3] = (value & 0xFF000000) >> 24;
    writeData(buffer, 4);
}

void HC595::writeData(const uint8_t* buffer, uint size) {
    if (!size || !buffer) { return; }
    if (_buffer.size() < size) { _buffer.resize(size); }
    if (_rckPin != 0xFF) { _gpio.resetPin(_rckPin); }
    buffer += size - 1;
    for (uint8_t* pData = _buffer.data() + size - 1;
         size != 0; size--, pData--, buffer--) {
        *pData = *buffer;
        if (_sinPin != 0xFF && _clkPin != 0xFF) {
            _gpio.resetPin(_clkPin);
            for (int bit = 7; bit >= 0; bit--) {
                _gpio.setPin(_sinPin, (*pData) & (1 << bit));
                sleep_us(_pulseTime);
                _gpio.setPin(_clkPin);
                sleep_us(_pulseTime);
                _gpio.resetPin(_clkPin);
            }
        }
        if (_rckPin != 0xFF) {
            _gpio.setPin(_rckPin);
            sleep_us(_pulseTime);
            _gpio.resetPin(_rckPin);
        }
    }
}

void HC595::setBit(uint bit, bool value) {
    uint index = bit / 8;
    if (index + 1 > _buffer.size()) {
        _buffer.resize(index + 1);
    }
    uint8_t data = _buffer[index];
    uint8_t mask = 0x01 << (bit - (index * 8));
    if (value) {
        data |= mask;
    } else {
        data &= ~mask;
    }
    _buffer[index] = data;
    writeData(_buffer.data(), _buffer.size());
}

void HC595::resetBit(uint bit) {
    setBit(bit, false);
}

const HC595::TData& HC595::getData(void) const {
    return _buffer;
}

const bool HC595::getBit(uint bit) const {
    uint index = bit / 8;
    uint8_t data = (index < _buffer.size())
        ? _buffer[index]
        : 0;
    uint8_t mask = 0x01 << (bit - (index * 8));
    return (data & mask);
}

const bool HC595::getOE(void) const {
    return _oe;
}
