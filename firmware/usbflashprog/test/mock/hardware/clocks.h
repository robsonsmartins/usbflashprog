// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------

#ifndef TEST_MOCK_HARDWARE_CLOCKS_H_
#define TEST_MOCK_HARDWARE_CLOCKS_H_

#include "pico/stdlib.h"

// ---------------------------------------------------------------------------

enum clock_index {
    clk_gpout0 = 0,
    clk_gpout1,
    clk_gpout2,
    clk_gpout3,
    clk_ref,
    clk_sys,
    clk_peri,
    clk_usb,
    clk_adc,
    clk_rtc,
    CLK_COUNT
};

// ---------------------------------------------------------------------------

extern "C" inline uint32_t clock_get_hz(clock_index clk_index) {
    return 125'000'000UL;
}

#endif  // TEST_MOCK_HARDWARE_CLOCKS_H_
