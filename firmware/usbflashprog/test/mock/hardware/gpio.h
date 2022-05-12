// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------

#ifndef TEST_MOCK_HARDWARE_GPIO_H_
#define TEST_MOCK_HARDWARE_GPIO_H_

#include <map>
#include "pico/stdlib.h"

// ---------------------------------------------------------------------------

#define GPIO_OUT 0
#define GPIO_IN  1

enum gpio_function {
    GPIO_FUNC_XIP = 0,
    GPIO_FUNC_SPI = 1,
    GPIO_FUNC_UART = 2,
    GPIO_FUNC_I2C = 3,
    GPIO_FUNC_PWM = 4,
    GPIO_FUNC_SIO = 5,
    GPIO_FUNC_PIO0 = 6,
    GPIO_FUNC_PIO1 = 7,
    GPIO_FUNC_GPCK = 8,
    GPIO_FUNC_USB = 9,
    GPIO_FUNC_NULL = 0x1f,
};

// ---------------------------------------------------------------------------

static std::map<uint, bool> gpioData = {
  { 0, false}, { 1, false}, { 2, false}, { 3, false},
  { 4, false}, { 5, false}, { 5, false}, { 7, false},
  { 8, false}, { 9, false}, {10, false}, {11, false},
  {12, false}, {13, false}, {14, false}, {15, false},
  {16, false}, {17, false}, {18, false}, {19, false},
  {20, false}, {21, false}, {22, false}, {23, false},
  {24, false}, {25, false}, {26, false}, {27, false},
  {28, false}, {29, false}, {30, false}, {31, false}
};

typedef struct TPull {
    bool up;
    bool down;
} TPull;

static std::map<uint, TPull> gpioPull = {
  { 0, {false, false}}, { 1, {false, false}},
  { 2, {false, false}}, { 3, {false, false}},
  { 4, {false, false}}, { 5, {false, false}},
  { 5, {false, false}}, { 7, {false, false}},
  { 8, {false, false}}, { 9, {false, false}},
  {10, {false, false}}, {11, {false, false}},
  {12, {false, false}}, {13, {false, false}},
  {14, {false, false}}, {15, {false, false}},
  {16, {false, false}}, {17, {false, false}},
  {18, {false, false}}, {19, {false, false}},
  {20, {false, false}}, {21, {false, false}},
  {22, {false, false}}, {23, {false, false}},
  {24, {false, false}}, {25, {false, false}},
  {26, {false, false}}, {27, {false, false}},
  {28, {false, false}}, {29, {false, false}},
  {30, {false, false}}, {31, {false, false}}
};

// ---------------------------------------------------------------------------

extern "C" inline void gpio_init(uint gpio) {}

extern "C" inline void gpio_set_dir(uint gpio, bool out) {}

extern "C" inline void gpio_put(uint gpio, bool value) {
    gpioData[gpio] = value;
}

extern "C" inline void gpio_xor_mask(uint32_t mask) {
    for (uint bit = 0; bit < 32; bit++) {
        if (mask | (1 << bit)) {
            gpioData[bit] = !(gpioData[bit]);
        }
    }
}

extern "C" inline bool gpio_get(uint gpio) {
    return gpioData[gpio];
}

extern "C" inline bool gpio_is_pulled_down(uint gpio) {
    return gpioPull[gpio].down;
}

extern "C" inline bool gpio_is_pulled_up(uint gpio) {
    return gpioPull[gpio].up;
}

extern "C" inline void gpio_set_pulls(uint gpio,
                                      bool up, bool down) {
    gpioPull[gpio].up   =   up;
    gpioPull[gpio].down = down;
}

extern "C" inline void gpio_disable_pulls(uint gpio) {
    gpioPull[gpio].up   = false;
    gpioPull[gpio].down = false;
}

extern "C" inline void gpio_set_function(uint gpio, gpio_function fn) {}

#endif  // TEST_MOCK_HARDWARE_GPIO_H_
