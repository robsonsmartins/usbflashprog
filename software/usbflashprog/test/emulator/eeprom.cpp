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
 * @file test/emulator/eeprom.cpp
 * @brief Implementation of EEPROM Chip Emulation.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "eeprom.hpp"

// ---------------------------------------------------------------------------

/* @brief Protect command sequence size. */
constexpr int kProtectCommandSize = 3;
/* @brief Unprotect command sequence size. */
constexpr int kUnprotectCommandSize = 6;

// clang-format off
/* @brief Command sequence to Protect an EEPROM 28C/X28 64
     (ST/Atmel/Xicor). */
constexpr TChipCommand kProtectCommandV1[kProtectCommandSize] = {
    {0x1555, 0xAA}, {0x0AAA, 0x55}, {0x1555, 0xA0}
};

/* @brief Command sequence to Unprotect an EEPROM 28C/X28 64
     (ST/Atmel/Xicor). */
constexpr TChipCommand kUnprotectCommandV1[kUnprotectCommandSize] = {
    {0x1555, 0xAA}, {0x0AAA, 0x55}, {0x1555, 0x80},
    {0x1555, 0xAA}, {0x0AAA, 0x55}, {0x1555, 0x20}
};

/* @brief Command sequence to Protect an EEPROM 28C/X28 256
     (ST/Atmel/Xicor). */
constexpr TChipCommand kProtectCommandV2[kProtectCommandSize] = {
    {0x5555, 0xAA}, {0x2AAA, 0x55}, {0x5555, 0xA0}
};

/* @brief Command sequence to Unprotect an EEPROM 28C/X28 256
     (ST/Atmel/Xicor). */
constexpr TChipCommand kUnprotectCommandV2[kUnprotectCommandSize] = {
    {0x5555, 0xAA}, {0x2AAA, 0x55}, {0x5555, 0x80},
    {0x5555, 0xAA}, {0x2AAA, 0x55}, {0x5555, 0x20}
};
// clang-format on

// ---------------------------------------------------------------------------

ChipEEPROM::ChipEEPROM()
    : BaseParChip(), f_commandStep(-1), f_commandOp(ChipOperationUnknown) {
    writeToLog("SetChip(%s)", "EEPROM");
}

ChipEEPROM::~ChipEEPROM() {}

void ChipEEPROM::setSize(uint32_t size) {
    if (size == f_memory_area.size()) return;
    /* inherited */
    BaseParChip::setSize(size);
    /* initialize with 0xFF */
    fillData(0xFFFF);
}

void ChipEEPROM::emuChip(void) {
    if (f_vdd && f_ce && !f_we && f_oe) {
        /* Read : VDD = 1; VPP = X; CE = 1; WE = 0; OE = 1; */
        read();
    } else if (f_vdd && f_ce && f_we) {
        /* Write: VDD = 1; VPP = X; CE = 1; WE = 1; OE = X; */
        // execute if special command or write
        if (!specialCommand()) write();
    } else if (!f_vdd || !f_ce) {
        /* VDD = 0 OR CE = 0 */
        // reset special command state
        if (f_commandStep != -1) {
            writeToLog("End of Special Command");
            f_commandStep = -1;
            f_commandOp = ChipOperationUnknown;
        }
    }
}

bool ChipEEPROM::specialCommand(void) {
    bool isCommand = false;
    int i = ((f_commandStep == 0xFF) ? -1 : f_commandStep) + 1;

    bool isUnprotect = (f_addr_bus == kUnprotectCommandV1[i].addr &&
                        f_data_bus == kUnprotectCommandV1[i].data) ||
                       (f_addr_bus == kUnprotectCommandV2[i].addr &&
                        f_data_bus == kUnprotectCommandV2[i].data);

    bool isProtect = (f_addr_bus == kProtectCommandV1[i].addr &&
                      f_data_bus == kProtectCommandV1[i].data) ||
                     (f_addr_bus == kProtectCommandV2[i].addr &&
                      f_data_bus == kProtectCommandV2[i].data);

    if (isUnprotect) {
        f_commandOp = ChipOperationUnprotect;
        isCommand = true;
    } else if (isProtect) {
        f_commandOp = ChipOperationProtect;
        isCommand = true;
    }
    if (isCommand) {
        if (f_commandStep != 0xFF) f_commandStep++;
        writeToLog("Special Command (addr %06X, data %02X)", f_addr_bus,
                   f_data_bus);
    }
    switch (f_commandOp) {
        case ChipOperationUnprotect:
            if (f_commandStep == (kUnprotectCommandSize - 1)) {
                // unprotect
                writeToLog("Special Command: Unprotect");
                f_commandStep = 0xFF;
                f_commandOp = ChipOperationUnknown;
            }
            break;
        case ChipOperationProtect:
            if (f_commandStep == (kProtectCommandSize - 1)) {
                // protect
                writeToLog("Special Command: Protect");
                f_commandStep = 0xFF;
                f_commandOp = ChipOperationUnknown;
            }
            break;
        default:
            break;
    }
    return isCommand;
}
