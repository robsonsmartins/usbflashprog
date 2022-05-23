// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------

#ifndef TEST_MOCK_HARDWARE_SYNC_H_
#define TEST_MOCK_HARDWARE_SYNC_H_

#include "pico/stdlib.h"

// ---------------------------------------------------------------------------

extern "C" inline uint32_t save_and_disable_interrupts(void) {
    return 0;
}

extern "C" inline void restore_interrupts(uint32_t status) {}

#endif  // TEST_MOCK_HARDWARE_SYNC_H_
