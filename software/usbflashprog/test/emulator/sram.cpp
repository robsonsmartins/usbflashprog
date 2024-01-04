// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2024) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup UnitTests
 * @file test/emulator/sram.cpp
 * @brief Implementation of SRAM Chip Emulation.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "sram.hpp"

// ---------------------------------------------------------------------------

ChipSRAM::ChipSRAM() : BaseParChip() {}

ChipSRAM::~ChipSRAM() {}

void ChipSRAM::setSize(uint32_t size) {
    /* inherited */
    BaseParChip::setSize(size);
    /* initialize with random contents */
    randomizeData();
}

void ChipSRAM::setVDD(bool state) {
    if (state == f_vdd) return;
    /* initialize with random contents */
    if (state) randomizeData();
    /* inherited */
    BaseParChip::setVDD(state);
}

void ChipSRAM::emuChip(void) {
    if (f_vdd && f_ce && !f_we && f_oe) {
        /* Read : VDD = 1; VPP = X; CE = 1; WE = 0; OE = 1; */
        read();
    } else if (f_vdd && f_ce && f_we) {
        /* Write: VDD = 1; VPP = X; CE = 1; WE = 1; OE = X; */
        write();
    }
}
