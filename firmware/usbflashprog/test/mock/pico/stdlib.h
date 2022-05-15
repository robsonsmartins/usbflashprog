// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------

#ifndef TEST_MOCK_PICO_STDLIB_H_
#define TEST_MOCK_PICO_STDLIB_H_

#include <unistd.h>
#include <cstdint>

// ---------------------------------------------------------------------------

typedef unsigned int uint;

#define __not_in_flash(group) __attribute__((section(".time_critical." group)))

#define __not_in_flash_func(func_name) \
    __not_in_flash(__STRING(func_name)) func_name

// ---------------------------------------------------------------------------

#define PICO_DEFAULT_LED_PIN 25

// ---------------------------------------------------------------------------

extern "C" inline void sleep_us(uint64_t us) {
    usleep(us);
}

extern "C" inline void sleep_ms(uint32_t ms) {
    usleep(static_cast<uint64_t>(ms) * 1000);
}

extern "C" inline void stdio_init_all(void) {}

extern "C" inline int getchar_timeout_us(uint32_t timeout_us) {
    usleep(timeout_us);
    return -1;
}

#endif  // TEST_MOCK_PICO_STDLIB_H_
