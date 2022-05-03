// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @file hal/gpio.cpp
 * @brief Implementation of the Pico GPIO Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "hal/gpio.hpp"

#include "hardware/gpio.h"

Gpio::Gpio() {}

void Gpio::setPin(uint pin, bool value) {
    _initPin(pin);
    gpio_set_dir(pin, GPIO_OUT);
    gpio_put(pin, value);
}

void Gpio::resetPin(uint pin) {
    setPin(pin, false);
}

void Gpio::togglePin(uint pin) {
    _initPin(pin);
    gpio_set_dir(pin, GPIO_OUT);
    uint32_t mask = 1ul << pin;
    gpio_xor_mask(mask);
}

bool Gpio::getPin(uint pin) {
    _initPin(pin);
    gpio_set_dir(pin, GPIO_IN);
    return (gpio_get(pin));
}

void Gpio::pullUp(uint pin, bool enable) {
    _initPin(pin);
    bool down = gpio_is_pulled_down(pin);
    gpio_set_pulls(pin, enable, down);
}

void Gpio::pullDown(uint pin, bool enable) {
    _initPin(pin);
    bool up = gpio_is_pulled_up(pin);
    gpio_set_pulls(pin, up, enable);
}

void Gpio::disablePulls(uint pin) {
    _initPin(pin);
    gpio_disable_pulls(pin);
}

void Gpio::_initPin(uint pin) {
    if (_initPins.find(pin) != _initPins.end()) { return; }
    gpio_init(pin);
    _initPins.insert(pin);
}
