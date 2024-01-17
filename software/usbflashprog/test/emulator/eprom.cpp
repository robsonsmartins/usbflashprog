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
 * @file test/emulator/eprom.cpp
 * @brief Implementation of EPROM Chip Emulation.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "eprom.hpp"

// ---------------------------------------------------------------------------

ChipEPROM::ChipEPROM()
    : BaseParChip(), f_numWriteFFAddrZero(0), f_numWriteAnother(0) {
    writeToLog("SetChip(%s)", "EPROM");
}

ChipEPROM::~ChipEPROM() {}

void ChipEPROM::setSize(uint32_t size) {
    /* inherited */
    BaseParChip::setSize(size);
    /* initialize with 0xFF */
    fillData(0xFFFF);
    f_numWriteFFAddrZero = 0;
    f_numWriteAnother = 0;
}

void ChipEPROM::write(void) {
    /* checks the params */
    if (f_addr_bus >= f_memory_area.size()) {
        writeToLog("Error in Write(addr=%06.6lX,data=%04.4X)", f_addr_bus,
                   f_data_bus);
        return;
    }
    static uint32_t last_addr = f_addr_bus;
    if (f_addr_bus == last_addr) {
        /* update full data */
        f_memory_area[f_addr_bus] = f_data_bus;
    } else {
        /* only bits 1 are changed */
        f_memory_area[f_addr_bus] &= f_data_bus;
    }
    /* update last_addr */
    last_addr = f_addr_bus;
    writeToLog("Write(addr=%06.6lX,data=%04.4X)", f_addr_bus, f_data_bus);
}

void ChipEPROM::emuChip(void) {
    // checks eprom WE polarity
    bool pgm = false;
    switch (f_memory_area.size()) {
        case 0x000800:  // 2KB - 2716
            // WE -> inverted
            pgm = !f_we;
            break;
        case 0x002000:  //   8KB - 2764
        case 0x004000:  //  16KB - 27128
        case 0x020000:  // 128KB - 27010/27C1024,27C210
        case 0x040000:  // 256KB - 27020/27C2048,27C220,27C202
            // WE -> direct
            pgm = f_we;
            break;
        case 0x200000:  // 2MB - 27C160
        case 0x400000:  // 4MB - 27C322
            // ignore WE state for read
            if (f_oe) {
                pgm = false;
            } else {
                pgm = f_we;
            }
            break;
        default:
            // WE -> inverted for read
            if (f_oe) {
                pgm = !f_we;
            } else {
                pgm = f_we;
            }
            break;
    }
    if (f_memory_area.size() < 0x000800) {
        // WE -> inverted for write
        if (f_oe) {
            pgm = f_we;
        } else {
            pgm = !f_we;
        }
    }
    if (f_vdd && f_ce && !pgm && f_oe) {
        // Read : VDD = 1; VPP = X; CE = 1; PGM = 0; OE = 1;
        writeToLog("About to Read...");
        read();
    } else if (f_vdd && f_vpp && f_ce && pgm && !f_oe) {
        // Write: VDD = 1; VPP = 1; CE = 1; PGM = 1; OE = 0;
        writeToLog("About to Write...");
        write();
        // Writing 0xFF more than 5 times at address 0x00 represents an
        // attempt to erase the memory
        if (f_addr_bus == 0 && (f_data_bus == 0xFF || f_data_bus == 0xFFFF)) {
            f_numWriteFFAddrZero++;
        } else {
            f_numWriteAnother++;
        }
        if (f_numWriteFFAddrZero > 5) {
            // erasing
            fillData(0xFFFF);
            f_numWriteFFAddrZero = 0;
            writeToLog("Erasing Chip");
        }
        // zering all counters
        if (f_numWriteAnother > 5) {
            f_numWriteAnother = 0;
            f_numWriteFFAddrZero = 0;
        }
    }
}
