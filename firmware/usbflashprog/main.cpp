// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/** 
 * @file main.cpp
 * @brief Implementation of the Main Routine.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <cstdio>
#include <cstring>
#include <iostream>

#include "pico/stdlib.h"

#include "hal/gpio.hpp"
#include "circuits/74hc595.hpp"

/**
 * @brief Main routine.
 * @return Error code (zero if success).
 */
int main() {
    stdio_init_all();

    Gpio gpio;
    HC595 hc595(12, 14, 15, 13);

    hc595.clear();
    hc595.outputEnable();

    uint8_t bit = 0;
    while (true) {
        gpio.togglePin(PICO_DEFAULT_LED_PIN);
        hc595.setBit(bit);
        sleep_ms(250);
        hc595.resetBit(bit);
        bit++;
        if (bit > 4) bit = 0;
    }

    return 0;
}
