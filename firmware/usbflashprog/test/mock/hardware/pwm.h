// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------

#ifndef TEST_MOCK_HARDWARE_PWM_H_
#define TEST_MOCK_HARDWARE_PWM_H_

#include "pico/stdlib.h"

// ---------------------------------------------------------------------------

extern "C" inline uint pwm_gpio_to_slice_num(uint gpio) {
    return gpio;
}

extern "C" inline uint pwm_gpio_to_channel(uint gpio) {
    return gpio;
}

extern "C" inline void pwm_set_clkdiv(uint slice_num, float divider) {}

extern "C" inline void pwm_set_wrap(uint slice_num, uint16_t wrap) {}

extern "C" inline void pwm_set_chan_level(uint slice_num, uint chan,
                                          uint16_t level) {}

extern "C" inline void pwm_set_enabled(uint slice_num, bool enabled) {}

#endif  // TEST_MOCK_HARDWARE_PWM_H_
