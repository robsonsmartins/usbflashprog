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

#include "usbflashprog.hpp"

// ---------------------------------------------------------------------------

/**
 * @brief Main routine.
 * @return Error code (zero if success).
 */
int main();

/** @brief UsbFlashProg instance. */
UsbFlashProg ufp;

// ---------------------------------------------------------------------------

int main() {
    ufp.init();
    while (true) {
        ufp.firstCore();
    }
    return 0;
}
