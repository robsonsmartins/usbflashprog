// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------

#ifndef TEST_MOCK_HARDWARE_ADC_H_
#define TEST_MOCK_HARDWARE_ADC_H_

#include "pico/stdlib.h"

constexpr const uint16_t RAW_ADC_DATA[2] = {0x221, 0xDDD};
static int8_t adcDataIndex = 0;

extern "C" inline void adc_init(void) {}

extern "C" inline void adc_select_input(uint input) {}

extern "C" inline uint16_t adc_read() {
    return (RAW_ADC_DATA[0] + RAW_ADC_DATA[1]) / 2;
}

extern "C" inline void adc_gpio_init(uint gpio) {}

extern "C" inline void adc_fifo_setup(bool en, bool dreq_en,
            uint16_t dreq_thresh, bool err_in_fifo, bool byte_shift) {}

extern "C" inline void adc_run(bool run) {}

extern "C" inline uint16_t adc_fifo_get_blocking() {
    if (adcDataIndex > 1) { adcDataIndex = 0; }
    return RAW_ADC_DATA[adcDataIndex++];
}

extern "C" inline void adc_fifo_drain() {}

#endif  // TEST_MOCK_HARDWARE_ADC_H_
