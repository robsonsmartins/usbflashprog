// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/** 
 * @defgroup Firmware Firmware Project
 * @brief    Firmware project for USB EPROM/Flash Programmer.
 */
// ---------------------------------------------------------------------------
/** 
 * @ingroup Firmware
 * @file main.cpp
 * @brief Implementation of the Main Routine.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <cstdio>
#include <cstring>
#include <iostream>
#include <iomanip>

#include "pico/stdlib.h"

#include "hal/gpio.hpp"
#include "hal/adc.hpp"
#include "hal/pwm.hpp"
#include "hal/multicore.hpp"
#include "circuits/dc2dc.hpp"
#include "circuits/74hc595.hpp"

void second_core(MultiCore& core); // NOLINT

MultiCore mcore(second_core);
Gpio gpio;
HC595 hc595(12, 14, 15, 13);
Dc2DcConfig vppConfig(
    21, 1, ((3900.0f + 470.0f) / 470.0f * 0.922916666f));
Dc2Dc vppGen(vppConfig);
uint8_t bit = 0;
float vpp = 0;

void second_core(MultiCore& core) { // NOLINT
    while (!core.isStopRequested()) {
        vppGen.adjust();
        vpp = vppGen.getV();
        std::cout << "VPP: " << std::fixed
            << std::setprecision(2) << vpp << "V" << std::endl;
    }
}

/**
 * @brief Main routine.
 * @return Error code (zero if success).
 */
int main() {
    stdio_init_all();

    hc595.clear();
    hc595.outputEnable();
    vppGen.start();

    vppGen.setV(12.0f);

    mcore.launch();

    while (true) {
        gpio.togglePin(PICO_DEFAULT_LED_PIN);
        hc595.setBit(bit);
        mcore.msleep(200);
        hc595.resetBit(bit);
        bit++;
        if (bit > 4) bit = 0;

        int opc = getchar_timeout_us(0);
        switch (opc) {
            case '+':
                break;
            case '-':
                break;
            case 'a':
                break;
            case 's':
                break;
            case '0':
                vppGen.stop();
                break;
            case '1':
                vppGen.start();
                break;
            default:
                break;
        }
    }

    return 0;
}
