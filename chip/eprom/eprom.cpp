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

ChipEPROM::ChipEPROM() : BaseParChip() {
    WriteToLog("SetChip(%s)", "EPROM");
}

ChipEPROM::~ChipEPROM() {}

void ChipEPROM::SetSize(unsigned long size) {
    if (size == f_memory_area.size()) {
        return;
    }
    /* initialize with 0xFF */
    FillData(0xFFFF);
    /* inherited */
    BaseParChip::SetSize(size);
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
    /* checks eprom WE polarity */
    bool eprom_we = false;
    switch (f_memory_area.size()) {  /* 8 bits / 16 bits */
        case 0x000800 /*    2048 */: /* 2716  */
            /* WE -> inverted */
            eprom_we = !f_we;
            break;
        case 0x001000 /*    4096 */: /* 2732  */
        case 0x008000 /*   32768 */: /* 27256 */
        case 0x010000 /*   65536 */: /* 27512 */
        case 0x080000 /*  524288 */: /* 27040  / 27C4096, 27C240, 27C4002,
                                        27C400 */
        case 0x100000 /* 1048576 */: /* 27080  / 27C800 */
            /* WE -> inverted for Read */
            if (f_oe) {
                eprom_we = !f_we;
            } else {
                eprom_we = f_we;
            }
            break;
        case 0x200000 /* 2097152 */: /* 27C160 */
        case 0x400000 /* 4194304 */: /* 27C322 */
            /* ignore WE state for Read */
            if (f_oe) {
                eprom_we = false;
            } else {
                eprom_we = f_we;
            }
            break;
        case 0x002000 /*    8192 */: /* 2764  */
        case 0x004000 /*   16384 */: /* 27128 */
        case 0x020000 /*  131072 */: /* 27010  / 27C1024, 27C210 */
        case 0x040000 /*  262144 */: /* 27020  / 27C2048, 27C220, 27C202 */
        default:
            /* WE -> direct */
            eprom_we = f_we;
            break;
    }
    if (f_memory_area.size() < 0x000800) {
        eprom_we = !f_we;
    }
    if (f_vdd && f_ce && !eprom_we && f_oe) {
        /* Read : VDD = 1; VPP = X; CE = 1; WE = 0; OE = 1; */
        Read();
    } else if (f_vdd && f_vpp && f_ce && eprom_we && !f_oe) {
        /* Write: VDD = 1; VPP = 1; CE = 1; WE = 1; OE = 0; */
        Write();
    }
}
