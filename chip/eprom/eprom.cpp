// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup lib_chip_eprom
 * @file    eprom.cpp
 * @brief   Implementation file (C++) for EPROM 27xxx Chip Emulation Library
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <vector>
#include "eprom.hpp"

// ---------------------------------------------------------------------------
/* internal */

/* global vars */

/* Chip object */
static ChipEPROM* objChip = NULL;

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
        objChip = new ChipEPROM();
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

ChipEPROM::ChipEPROM()
    : BaseParChip(), f_numWriteFFAddrZero(0), f_numWriteAnother(0) {
    WriteToLog("SetChip(%s)", "EPROM");
}

ChipEPROM::~ChipEPROM() {}

void ChipEPROM::SetSize(unsigned long size) {
    if (size == f_memory_area.size()) return;
    /* inherited */
    BaseParChip::SetSize(size);
    /* initialize with 0xFF */
    FillData(0xFFFF);
    f_numWriteFFAddrZero = 0;
    f_numWriteAnother = 0;
}

void ChipEPROM::Write(void) {
    /* checks the params */
    if (f_addr_bus >= f_memory_area.size()) {
        WriteToLog("Error in Write(addr=%06.6lX,data=%04.4X)", f_addr_bus,
                   f_data_bus);
        return;
    }
    static unsigned long last_addr = f_addr_bus;
    if (f_addr_bus == last_addr) {
        /* update full data */
        f_memory_area[f_addr_bus] = f_data_bus;
    } else {
        /* only bits 1 are changed */
        f_memory_area[f_addr_bus] &= f_data_bus;
    }
    /* update last_addr */
    last_addr = f_addr_bus;
    /* log */
    WriteToLog("Write(addr=%06.6lX,data=%04.4X)", f_addr_bus, f_data_bus);
}

void ChipEPROM::EmuChip(void) {
    /* for write data:
       27(C)16
       vdd = true; vpp = true; ce = true; we = false; oe = false;
       27(C)xxx
       vdd = true; vpp = true; ce = true; we = true;  oe = false;
    */
    /* for read data:
       16/32/256/512/040/080
       vdd = true; vpp = X; ce = true; we = true;  oe = true;
       64/128/010/020/...
       vdd = true; vpp = X;  ce = true; we = false; oe = true;
       160/322 (16 bit)
       vdd = true; vpp = X;  ce = true; we = X; oe = true;
    */
    // Checks eprom WE polarity
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
        pgm = !f_we;
    }
    if (f_vdd && f_ce && !pgm && f_oe) {
        // Read : VDD = 1; VPP = X; CE = 1; WE = 0; OE = 1;
        Read();
    } else if (f_vdd && f_vpp && f_ce && pgm && !f_oe) {
        // Write: VDD = 1; VPP = 1; CE = 1; WE = 1; OE = 0;
        Write();
        // Writing 0xFF more than 5 times at address 0x00 represents an
        // attempt to erase the memory
        if (f_addr_bus == 0 && (f_data_bus == 0xFF || f_data_bus == 0xFFFF)) {
            f_numWriteFFAddrZero++;
        } else {
            f_numWriteAnother++;
        }
        if (f_numWriteFFAddrZero > 5) {
            // Erasing
            FillData(0xFFFF);
            f_numWriteFFAddrZero = 0;
            WriteToLog("Erasing Chip");
        }
        // Zering all counters
        if (f_numWriteAnother > 5) {
            f_numWriteAnother = 0;
            f_numWriteFFAddrZero = 0;
        }
    }
}
