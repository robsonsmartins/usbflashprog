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
#include "hal/adc.hpp"
#include "hal/pwm.hpp"
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

    Adc adc;

    Pwm pwm(20);

    uint8_t bit = 0;
    float vpp;
    float duty = 0;
    pwm.setFreq(40000);
    pwm.start();
    while (true) {
        gpio.togglePin(PICO_DEFAULT_LED_PIN);
        hc595.setBit(bit);
        sleep_ms(200);
        hc595.resetBit(bit);
        bit++;
        if (bit > 4) bit = 0;
        vpp = adc.capture(1, 1024);
        vpp = vpp / 470 * (3900 + 470);
        vpp *= 0.97173913f;

        int opc = getchar_timeout_us(0);
        switch (opc) {
            case '+':
                duty += 0.5f;
                if (duty > 100.0f) { duty = 100.0f; }
                pwm.setDuty(duty);
                break;
            case '-':
                duty -= 0.5f;
                if (duty < 0.0f) { duty = 0.0f; }
                pwm.setDuty(duty);
                break;
            case 'a':
                pwm.setFreq(20000);
                break;
            case 's':
                pwm.setFreq(40000);
                break;
            case '0':
                pwm.stop();
                break;
            case '1':
                pwm.start();
                break;
            default:
                break;
        }
        // std::cout << "VPP: " << vpp << "V" << std::endl;
    }

    return 0;
}
