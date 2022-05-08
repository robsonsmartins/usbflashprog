// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @file hal/adc.cpp
 * @brief Implementation of the Pico ADC Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <vector>

#include "hal/adc.hpp"

#include "hardware/gpio.h"
#include "hardware/adc.h"

#define GPIO_PIN_ADC_CHANNEL_0 26
#define ADC_MAX_CHANNEL 3

/** @cond */
float __not_in_flash_func(adc_capture)(uint channel,
                                       uint16_t *buf, size_t size);
/** @endcond */

Adc::Adc(): vref_(DEFAULT_VREF) {
    adc_init();
}

Adc::Adc(float vref): vref_(vref) {
    adc_init();
}

float Adc::capture(uint channel) {
    if (!initChannel_(channel)) { return -1.0f; }
    adc_select_input(channel);
    return calculate_(adc_read());
}

float Adc::capture(uint channel, size_t size) {
    if (!size || !initChannel_(channel)) { return -1.0f; }
    adc_select_input(channel);
    std::vector<uint16_t> raw(size);
    return calculate_(adc_capture(channel, raw.data(), size));
}

float Adc::capture(uint channel, float* buf, size_t size) {
    if (!size || !buf || !initChannel_(channel)) { return -1.0f; }
    adc_select_input(channel);
    std::vector<uint16_t> raw(size);
    float result = calculate_(adc_capture(channel, raw.data(), size));
    for (size_t i = 0; i < size; i++) {
        buf[i] = calculate_(raw[i]);
    }
    return result;
}

bool Adc::initChannel_(uint channel) {
    if (channel > ADC_MAX_CHANNEL) { return false; }
    if (initChannels_.find(channel) != initChannels_.end()) { return true; }
    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(channel + GPIO_PIN_ADC_CHANNEL_0);
    initChannels_.insert(channel);
    return true;
}

float Adc::calculate_(uint16_t value) {
    float result = value * vref_ / (1 << 12);
    return result;
}

/** @cond */
float __not_in_flash_func(adc_capture)(uint channel,
                                       uint16_t *buf, size_t size) {
    adc_fifo_setup(true, false, 0, false, false);
    adc_run(true);
    float mbuf = 0.0f;
    for (int i = 0; i < size; i = i + 1) {
        buf[i] = adc_fifo_get_blocking();
        mbuf += buf[i];
    }
    adc_run(false);
    adc_fifo_drain();
    return (mbuf / size);
}
/** @endcond */
