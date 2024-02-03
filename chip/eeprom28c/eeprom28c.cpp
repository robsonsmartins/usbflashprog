// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup lib_chip_eeprom28c
 * @file    eeprom28c.cpp
 * @brief   Implementation file (C++) for EEPROM 28Cxxx Chip Emulation Library
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "eeprom28c.hpp"

// ---------------------------------------------------------------------------
/* internal */

/* global vars */
/* Chip object */
static ChipEEPROM28C* objChip = NULL;

// ---------------------------------------------------------------------------
/* functions */
static bool _Load(void);
static void _Unload(void);

// ---------------------------------------------------------------------------
/* implementation */

DLLEXPORT void SetSize(unsigned long size) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    objChip->SetSize(size);
}

DLLEXPORT void SetVDD(unsigned char state) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    objChip->SetVDD(state);
}

DLLEXPORT void SetVPP(unsigned char state) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    objChip->SetVPP(state);
}

DLLEXPORT void SetOE(unsigned char state) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    objChip->SetOE(state);
}

DLLEXPORT void SetCE(unsigned char state) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    objChip->SetCE(state);
}

DLLEXPORT void SetWE(unsigned char state) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    objChip->SetWE(state);
}

DLLEXPORT void SetAddrBus(unsigned long addr) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    objChip->SetAddrBus(addr);
}

DLLEXPORT void SetDataBus(unsigned short data) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    objChip->SetDataBus(data);
}

DLLEXPORT unsigned short GetDataBus(void) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return 0xFFFF;
    }
    /* calls method in chip object */
    return (objChip->GetDataBus());
}

DLLEXPORT void SerialSetCS(unsigned char state) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    // nothing for BaseParChip derivated
    // objChip->SerialSetCS(state);
}

DLLEXPORT void SerialSetClk(unsigned char state) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    // nothing for BaseParChip derivated
    // objChip->SerialSetClk(state);
}

DLLEXPORT void SerialSetData(unsigned char state) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    // nothing for BaseParChip derivated
    // objChip->SerialSetData(state);
}

DLLEXPORT unsigned char SerialGetData(void) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return 1;
    }
    /* calls method in chip object */
    // nothing for BaseParChip derivated
    // return ((objChip->SerialGetData()) ? 1 : 0);
    return 1;
}

/* DLL entry point for MSWindows */
BOOL WINAPI DllMain(HINSTANCE hInst,   /* Library instance handle. */
                    DWORD reason,      /* Reason this is being called. */
                    LPVOID reserved) { /* Not used. */
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            _Load();
            break;
        case DLL_PROCESS_DETACH:
            _Unload();
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
    }
    /* Returns TRUE on success, FALSE on failure */
    return TRUE;
}

// ---------------------------------------------------------------------------

bool _Load(void) {
    /* if already loaded, then exits */
    if (objChip != NULL) {
        return true;
    }
    /* creates a Chip object */
    try {
        objChip = new ChipEEPROM28CWorkaround();
    } catch (std::bad_alloc&) {
        objChip = NULL;
    }
    /* check for errors */
    if (objChip == NULL) {
        _Unload();
        return false;
    }
    /* returns */
    return true;
}

void _Unload(void) {
    /* destroies the Chip object */
    if (objChip != NULL) {
        delete objChip;
        objChip = NULL;
    }
}

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

ChipEEPROM28C::ChipEEPROM28C()
    : BaseParChip(), f_commandStep(-1), f_commandOp(ChipOperationUnknown) {
    WriteToLog("SetChip(%s)", "EEPROM28C");
}

ChipEEPROM28C::~ChipEEPROM28C() {}

void ChipEEPROM28C::SetSize(unsigned long size) {
    if (size == f_memory_area.size()) return;
    /* inherited */
    BaseParChip::SetSize(size);
    /* initialize with 0xFF */
    FillData(0xFF);
}

void ChipEEPROM28C::EmuChip(void) {
    if (f_vdd && f_ce && !f_we && f_oe) {
        /* Read : VDD = 1; VPP = X; CE = 1; WE = 0; OE = 1; */
        Read();
    } else if (f_vdd && f_ce && f_we) {
        /* Write: VDD = 1; VPP = X; CE = 1; WE = 1; OE = X; */
        // execute if special command or write
        if (!SpecialCommand()) Write();
    } else if (!f_vdd || !f_ce) {
        /* VDD = 0 OR CE = 0 */
        // reset special command state
        if (f_commandStep != -1) {
            WriteToLog("End of Special Command");
            f_commandStep = -1;
            f_commandOp = ChipOperationUnknown;
        }
    }
}

bool ChipEEPROM28C::SpecialCommand(void) {
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
        WriteToLog("Special Command (addr %06X, data %02X)", f_addr_bus,
                   f_data_bus);
    }
    switch (f_commandOp) {
        case ChipOperationUnprotect:
            if (f_commandStep == (kUnprotectCommandSize - 1)) {
                // unprotect
                WriteToLog("Special Command: Unprotect");
                f_commandStep = 0xFF;
                f_commandOp = ChipOperationUnknown;
            }
            break;
        case ChipOperationProtect:
            if (f_commandStep == (kProtectCommandSize - 1)) {
                // protect
                WriteToLog("Special Command: Protect");
                f_commandStep = 0xFF;
                f_commandOp = ChipOperationUnknown;
            }
            break;
        default:
            break;
    }
    return isCommand;
}

// ---------------------------------------------------------------------------

ChipEEPROM28CWorkaround::ChipEEPROM28CWorkaround()
    : ChipEEPROM28C(), f_address(0), f_readCount(0) {}

ChipEEPROM28CWorkaround::~ChipEEPROM28CWorkaround() {}

void ChipEEPROM28CWorkaround::EmuChip(void) {
    if (f_vdd && f_ce && !f_we && f_oe) {
        /* Read : VDD = 1; VPP = X; CE = 1; WE = 0; OE = 1; */
        Read();
        // count reads
        f_readCount++;
        // if sequencial reads up to 64
        if (f_readCount == 65) {
            // reset the special command state
            WriteToLog("End of Special Command");
            f_commandStep = -1;
            f_commandOp = ChipOperationUnknown;
        }
    } else if (f_vdd && f_ce && f_we) {
        /* Write: VDD = 1; VPP = X; CE = 1; WE = 1; OE = X; */
        // check if is special command
        bool isCommand = (f_addr_bus == kProtectCommandV1[0].addr ||
                          f_addr_bus == kProtectCommandV2[0].addr) &&
                         (f_commandStep == 0xFF);
        // if is special command, fix the address
        if (isCommand) f_addr_bus = f_address;
        // execute if special command or write
        if (!SpecialCommand()) Write();
        // if is special command, increment the (fixup) address
        if (isCommand) f_address++;
        // reset read counter (because is a write)
        f_readCount = 0;
    } else if (!f_vdd || !f_ce) {
        /* VDD = 0 OR CE = 0 */
        // reset special command state
        if (f_commandStep != -1) {
            WriteToLog("End of Special Command");
            f_commandStep = -1;
            f_commandOp = ChipOperationUnknown;
            // reset the (fixup) address
            f_address = 0;
        }
    }
}
