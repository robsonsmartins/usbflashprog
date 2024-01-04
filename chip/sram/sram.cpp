// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup lib_chip_sram
 * @file    sram.cpp
 * @brief   Implementation file (C++) for SRAM Chip Emulation Library
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "sram.hpp"

// ---------------------------------------------------------------------------
/* internal */
/* global vars */

/* Chip object */
static ChipSRAM* objChip = NULL;

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
        objChip = new ChipSRAM();
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

ChipSRAM::ChipSRAM() : BaseParChip() {
    WriteToLog("SetChip(%s)", "SRAM");
}

ChipSRAM::~ChipSRAM() {}

void ChipSRAM::SetSize(unsigned long size) {
    if (size == f_memory_area.size()) return;
    /* inherited */
    BaseParChip::SetSize(size);
    /* initialize with random contents */
    RandomizeData();
}

void ChipSRAM::SetVDD(bool state) {
    if (state == f_vdd) return;
    /* initialize with random contents */
    if (state) RandomizeData();
    /* inherited */
    BaseParChip::SetVDD(state);
}

void ChipSRAM::EmuChip(void) {
    if (f_vdd && f_ce && !f_we && f_oe) {
        /* Read : VDD = 1; VPP = X; CE = 1; WE = 0; OE = 1; */
        Read();
    } else if (f_vdd && f_ce && f_we) {
        /* Write: VDD = 1; VPP = X; CE = 1; WE = 1; OE = X; */
        Write();
    }
}
