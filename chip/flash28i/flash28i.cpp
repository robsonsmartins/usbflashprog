// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2024) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup lib_chip_flash28i
 * @file    flash28i.cpp
 * @brief   Implementation file (C++) for Flash Intel 28Fxxx Chip Emulation
 *          Library
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "flash28i.hpp"

// ---------------------------------------------------------------------------
/* internal */

/* global vars */
/* Chip object */
static ChipFlashIntel28F* objChip = NULL;

// ---------------------------------------------------------------------------
/* functions */
static bool _Load(void);
static void _Unload(void);

// ---------------------------------------------------------------------------
/* implementation */

DLLEXPORT void SetSize(uint32_t size) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    objChip->SetSize(size);
}

DLLEXPORT void SetVDD(uint8_t state) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    objChip->SetVDD(state);
}

DLLEXPORT void SetVPP(uint8_t state) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    objChip->SetVPP(state);
}

DLLEXPORT void SetOE(uint8_t state) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    objChip->SetOE(state);
}

DLLEXPORT void SetCE(uint8_t state) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    objChip->SetCE(state);
}

DLLEXPORT void SetWE(uint8_t state) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    objChip->SetWE(state);
}

DLLEXPORT void SetAddrBus(uint32_t addr) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    objChip->SetAddrBus(addr);
}

DLLEXPORT void SetDataBus(uint16_t data) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    objChip->SetDataBus(data);
}

DLLEXPORT uint16_t GetDataBus(void) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return 0xFFFF;
    }
    /* calls method in chip object */
    return (objChip->GetDataBus());
}

DLLEXPORT void SerialSetCS(uint8_t state) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    // nothing for BaseParChip derivated
    // objChip->SerialSetCS(state);
}

DLLEXPORT void SerialSetClk(uint8_t state) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    // nothing for BaseParChip derivated
    // objChip->SerialSetClk(state);
}

DLLEXPORT void SerialSetData(uint8_t state) {
    /* initialize objects */
    if (!_Load() || objChip == NULL) {
        return;
    }
    /* calls method in chip object */
    // nothing for BaseParChip derivated
    // objChip->SerialSetData(state);
}

DLLEXPORT uint8_t SerialGetData(void) {
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
        objChip = new ChipFlashIntel28F();
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

/* @brief Hardcoded (emulated) Manufacturer ID: Intel. */
constexpr uint8_t kChipManufacturerId = 0x89;
/* @brief Hardcoded (emulated) Device ID: 28F001BX-T. */
constexpr uint8_t kChipDeviceId = 0x94;
/* @brief Hardcoded (emulated) Chip Status Byte. */
constexpr uint8_t kChipStatusByte = 0x80;

// clang-format off

/* Intel 28F Chip commands. */
/* @brief Intel 28F Read command. */
constexpr uint8_t kReadCmd28i [] = { 0xFF };
/* @brief Intel 28F Write command. */
constexpr uint8_t kWriteCmd28i[] = { 0x40 };
/* @brief Intel 28F Erase device command. */
constexpr uint8_t kEraseCmd28i[] = { 0x20, 0xD0 };
/* @brief Intel 28F GetID command. */
constexpr uint8_t kGetIdCmd28i[] = { 0x90 };

// clang-format on

// ---------------------------------------------------------------------------

ChipFlashIntel28F::ChipFlashIntel28F()
    : BaseParChip(), f_commandStep(-1), f_operation(ChipOperationRead) {
    WriteToLog("SetChip(%s)", "Flash i28F");
}

ChipFlashIntel28F::~ChipFlashIntel28F() {}

void ChipFlashIntel28F::SetSize(uint32_t size) {
    if (size == f_memory_area.size()) return;
    /* inherited */
    BaseParChip::SetSize(size);
    /* initialize with 0xFF */
    FillData(0xFF);
    f_commandStep = -1;
    f_operation = ChipOperationRead;
}

uint16_t ChipFlashIntel28F::GetDataBus(void) {
    // updates the data bus
    UpdateDataBus(false);
    // returns the data bus
    return f_data_bus;
}

bool ChipFlashIntel28F::UpdateDataBus(bool readMemory) {
    bool isExecuteCmd = (f_commandStep == 0xFF);
    bool isGetId = (f_operation == ChipOperationGetId);
    bool isWrite = (f_operation == ChipOperationWrite);
    bool isErase = (f_operation == ChipOperationErase);
    if (f_vdd && f_ce && !f_we && f_oe) {
        /* Read/GetID/Read StatusByte:
           VDD = 1; VPP = X; CE = 1; WE = 0; OE = 1; */
        if (isExecuteCmd && isGetId) {
            // GetID
            if (f_addr_bus == 0x00 && f_data_bus != kChipManufacturerId) {
                // return manufacturer ID
                f_data_bus = kChipManufacturerId;
                WriteToLog("Manufacturer ID = %02X", f_data_bus);
            } else if (f_addr_bus == 0x01 && f_data_bus != kChipDeviceId) {
                // reutrn device ID
                f_data_bus = kChipDeviceId;
                WriteToLog("Device ID = %02X", f_data_bus);
                return true;
            }
        } else if (isExecuteCmd && (isWrite || isErase)) {
            // Write or Erase
            // return Status Byte
            f_data_bus = kChipStatusByte;
            WriteToLog("Status Byte = %02X", f_data_bus);
            return true;
        } else if (readMemory) {
            // Read
            Read();
            return true;
        }
    }
    return false;
}

void ChipFlashIntel28F::EmuChip(void) {
    bool isExecuteCmd = (f_commandStep == 0xFF);
    bool isWrite = (f_operation == ChipOperationWrite);
    if (f_vdd && f_ce && !f_we && f_oe) {
        /* Read/GetID/Read StatusByte:
           VDD = 1; VPP = X; CE = 1; WE = 0; OE = 1; */
        // Update DataBus/Read
        if (UpdateDataBus()) {
            f_commandStep = -1;
            f_operation = ChipOperationRead;
        }
    } else if (f_vdd && f_ce && f_we) {
        /* Command/Write/Erase:
           VDD = 1; VPP = X; CE = 1; WE = 1; OE = X; */
        if (isExecuteCmd && isWrite) {
            // Write
            Write();
        } else {
            // Handle special commands
            if (!SpecialCommand()) Write();
        }
    } else if (!f_vdd || !f_ce) {
        /* VDD = 0 OR CE = 0 */
        // reset special command state
        if (f_commandStep != -1) {
            f_commandStep = -1;
            f_operation = ChipOperationRead;
        }
    }
}

bool ChipFlashIntel28F::SpecialCommand(void) {
    if (f_commandStep == 0xFF) f_commandStep = -1;
    unsigned int index = f_commandStep + 1;
    uint8_t data = f_data_bus & 0xFF;

    bool isRead = (index < sizeof(kReadCmd28i) && data == kReadCmd28i[index]);
    bool isWrite =
        (index < sizeof(kWriteCmd28i) && data == kWriteCmd28i[index]);
    bool isGetId =
        (index < sizeof(kGetIdCmd28i) && data == kGetIdCmd28i[index]);
    bool isErase =
        (index < sizeof(kEraseCmd28i) && data == kEraseCmd28i[index]);

    if (isRead) {
        WriteToLog("Command: %02X (Read)", data);
        f_operation = ChipOperationRead;
        f_commandStep = 0xFF;
        return true;
    }
    if (isWrite) {
        WriteToLog("Command: %02X (Write)", data);
        f_operation = ChipOperationWrite;
        f_commandStep = 0xFF;
        return true;
    }
    if (isGetId) {
        WriteToLog("Command: %02X (GetID)", data);
        f_operation = ChipOperationGetId;
        f_commandStep = 0xFF;
        return true;
    }
    if (isErase) {
        f_operation = ChipOperationErase;
        if (index == 0) {
            WriteToLog("Write Command (cycle #1): %02X", data);
            f_commandStep++;
        } else if (index == 1) {
            WriteToLog("Command: %02X (Erase)", data);
            // Erase chip
            WriteToLog("Erasing chip...");
            FillData(0xFF);
            f_commandStep = 0xFF;
        }
        return true;
    }
    WriteToLog("Write Command: %02X (Invalid)", data);
    return false;
}
