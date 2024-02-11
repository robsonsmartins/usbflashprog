// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2024) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup lib_chip_flash28f
 * @file    flash28sst.cpp
 * @brief   Implementation file (C++) for Flash SST 28Fxxx Chip Emulation
 *          Library
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "flash28sst.hpp"

// ---------------------------------------------------------------------------
/* internal */

/* global vars */
/* Chip object */
static ChipFlashSST28F* objChip = NULL;

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
        objChip = new ChipFlashSST28F();
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

/* @brief Hardcoded (emulated) Manufacturer ID: SST. */
constexpr uint8_t kChipManufacturerId = 0xBF;
/* @brief Hardcoded (emulated) Device ID: SST28SF/VF040A. */
constexpr uint8_t kChipDeviceId = 0x04;

// clang-format off

/* 28F Chip commands. */
/* @brief 28F Read command. */
constexpr uint8_t kReadCmd28F      [] = { 0x00 };
/* @brief 28F Write command. */
constexpr uint8_t kWriteCmd28F     [] = { 0x40 };
/* @brief 28F Verify command (after write). */
constexpr uint8_t kVerifyCmd28F    [] = { 0xC0 };
/* @brief 28F Erase device command. */
constexpr uint8_t kEraseCmd28F     [] = { 0x20, 0x20 };
/* @brief 28F Blank Check command (after erase). */
constexpr uint8_t kBlankCheckCmd28F[] = { 0xA0 };
/* @brief 28F GetID command. */
constexpr uint8_t kGetIdCmd28F     [] = { 0x90 };
/* @brief 28F Reset command (goto Read). */
constexpr uint8_t kResetCmd28F     [] = { 0xFF, 0xFF };

/* Am28FxA Chip commands. */
/* @brief Am28FxA Read command. */
constexpr uint8_t kReadCmdAm28F  [] = { 0x00 };
/* @brief Am28FxA Write command. */
constexpr uint8_t kWriteCmdAm28F [] = { 0x50 };
/* @brief Am28FxA Write command 2. */
constexpr uint8_t kWriteCmd2Am28F[] = { 0x10 };
/* @brief Am28FxA Erase device command. */
constexpr uint8_t kEraseCmdAm28F [] = { 0x30, 0x30 };
/* @brief Am28FxA GetID command. */
constexpr uint8_t kGetIdCmdAm28F [] = { 0x90 };
/* @brief Am28FxA GetID command 2. */
constexpr uint8_t kGetIdCmd2Am28F[] = { 0x80 };
/* @brief Am28FxA Reset command (goto Read). */
constexpr uint8_t kResetCmdAm28F [] = { 0xFF, 0xFF };

/* Intel 28F Chip commands. */
/* @brief Intel 28F Read command. */
constexpr uint8_t kReadCmdI28F [] = { 0xFF };
/* @brief Intel 28F Write command. */
constexpr uint8_t kWriteCmdI28F[] = { 0x40 };
/* @brief Intel 28F Erase device command. */
constexpr uint8_t kEraseCmdI28F[] = { 0x20, 0xD0 };
/* @brief Intel 28F GetID command. */
constexpr uint8_t kGetIdCmdI28F[] = { 0x90 };

/* @brief SST 28xF Protect/Unprotect device sequence size. */
constexpr uint8_t kProtectSST28SFSize = 7;
/* @brief SST 28xF Unprotect device addresses. */
constexpr uint16_t kUnprotectSST28SF[] = { 
    0x1823, 0x1820, 0x1822, 0x0418, 0x041B, 0x0419, 0x041A 
};
/* @brief SST 28xF Protect device addresses. */
constexpr uint16_t kProtectSST28SF[] = { 
    0x1823, 0x1820, 0x1822, 0x0418, 0x041B, 0x0419, 0x040A
};

// clang-format on

// ---------------------------------------------------------------------------

ChipFlashSST28F::ChipFlashSST28F()
    : BaseParChip(),
      f_commandStep(-1),
      f_protected(true),
      f_operation(ChipOperationRead) {
    WriteToLog("SetChip(%s)", "Flash SST28xF");
}

ChipFlashSST28F::~ChipFlashSST28F() {}

void ChipFlashSST28F::SetSize(uint32_t size) {
    if (size == f_memory_area.size()) return;
    /* inherited */
    BaseParChip::SetSize(size);
    /* initialize with 0xFF */
    FillData(0xFF);
    f_commandStep = -1;
    f_operation = ChipOperationRead;
}

void ChipFlashSST28F::SetVDD(bool state) {
    bool oldVdd = f_vdd;
    /* inherited */
    BaseParChip::SetVDD(state);
    // SDP enable in poweron
    if (!oldVdd && state) f_protected = true;
}

void ChipFlashSST28F::Read(void) {
    static uint32_t last_addr = static_cast<uint32_t>(-1);
    uint16_t data =
        (f_addr_bus < f_memory_area.size()) ? f_memory_area[f_addr_bus] : 0xFF;
    bool isSpecialCmd = (f_operation == ChipOperationUnprotect ||
                         f_operation == ChipOperationProtect) &&
                        f_commandStep != -1;
    if ((isSpecialCmd || f_data_bus == data) && f_addr_bus == last_addr) return;
    if (f_commandStep == 0xFF) f_commandStep = -1;
    unsigned int index = f_commandStep + 1;
    if (f_addr_bus == kUnprotectSST28SF[index]) {
        WriteToLog("Unprotect (step #%d/%d)", index + 1, kProtectSST28SFSize);
        f_commandStep++;
        f_operation = ChipOperationUnprotect;
    } else if (f_addr_bus == kProtectSST28SF[index]) {
        WriteToLog("Protect (step #%d/%d)", index + 1, kProtectSST28SFSize);
        f_commandStep++;
        f_operation = ChipOperationProtect;
    } else if (f_operation == ChipOperationUnprotect ||
               f_operation == ChipOperationProtect) {
        WriteToLog("Cancel Protect/Unprotect sequence. Addr: %06X.",
                   f_addr_bus);
        f_commandStep = -1;
        f_operation = ChipOperationRead;
    }
    if ((f_commandStep + 1) >= kProtectSST28SFSize) {
        if (f_operation == ChipOperationProtect) {
            WriteToLog("Protecting Device (Enabling SDP)");
        } else {
            WriteToLog("Unprotecting Device (Disabling SDP)");
        }
        f_protected = (f_operation == ChipOperationProtect);
        f_commandStep = -1;
        f_operation = ChipOperationRead;
    }
    // returns the data from memory area
    f_data_bus = data;
    // update last_addr
    last_addr = f_addr_bus;
    WriteToLog("Read(addr=%06.6lX) = %04.4X", f_addr_bus, f_data_bus);
}

void ChipFlashSST28F::EmuChip(void) {
    bool isExecuteCmd = (f_commandStep == 0xFF);
    bool isGetId = (f_operation == ChipOperationGetId);
    bool isWrite = (f_operation == ChipOperationWrite);
    if (f_vdd && f_ce && !f_we && f_oe) {
        /* Read/GetID:
           VDD = 1; VPP = X; CE = 1; WE = 0; OE = 1; */
        if (isExecuteCmd && isGetId) {
            // GetID
            if (f_addr_bus == 0x00 && f_data_bus != kChipManufacturerId) {
                // return manufacturer ID
                f_data_bus = kChipManufacturerId;
                WriteToLog("Manufacturer ID = %02X", f_data_bus);
            } else if (f_addr_bus == 0x01 && f_data_bus != kChipDeviceId) {
                // return device ID
                f_data_bus = kChipDeviceId;
                WriteToLog("Device ID = %02X", f_data_bus);
                f_commandStep = -1;
                f_operation = ChipOperationRead;
            }
        } else {
            // Read
            Read();
        }
    } else if (f_vdd && f_ce && f_we) {
        /* Command/Write/Erase:
           VDD = 1; VPP = X; CE = 1; WE = 1; OE = X; */
        if (isExecuteCmd && isWrite) {
            // Write
            if (f_protected) {
                WriteToLog("Write Error! Device is protected (SDP enabled)");
            } else {
                Write();
            }
            f_commandStep = -1;
            f_operation = ChipOperationRead;
        } else {
            // Handle special commands
            if (!SpecialCommand()) {
                if (f_protected) {
                    WriteToLog(
                        "Write Error! Device is protected (SDP enabled)");
                } else {
                    Write();
                }
            }
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

bool ChipFlashSST28F::SpecialCommand(void) {
    if (f_commandStep == 0xFF) f_commandStep = -1;
    unsigned int index = f_commandStep + 1;
    uint8_t data = f_data_bus & 0xFF;

    bool isRead =
        (index < sizeof(kReadCmd28F) && data == kReadCmd28F[index]) ||
        (index < sizeof(kReadCmdAm28F) && data == kReadCmdAm28F[index]) ||
        (index < sizeof(kReadCmdI28F) && data == kReadCmdI28F[index]);
    bool isWrite =
        (index < sizeof(kWriteCmd28F) && data == kWriteCmd28F[index]) ||
        (index < sizeof(kWriteCmdAm28F) && data == kWriteCmdAm28F[index]) ||
        (index < sizeof(kWriteCmd2Am28F) && data == kWriteCmd2Am28F[index]) ||
        (index < sizeof(kWriteCmdI28F) && data == kWriteCmdI28F[index]);
    bool isVerify =
        (index < sizeof(kVerifyCmd28F) && data == kVerifyCmd28F[index]);
    bool isBlankCheck =
        (index < sizeof(kBlankCheckCmd28F) && data == kBlankCheckCmd28F[index]);
    bool isGetId =
        (index < sizeof(kGetIdCmd28F) && data == kGetIdCmd28F[index]) ||
        (index < sizeof(kGetIdCmdAm28F) && data == kGetIdCmdAm28F[index]) ||
        (index < sizeof(kGetIdCmd2Am28F) && data == kGetIdCmd2Am28F[index]) ||
        (index < sizeof(kGetIdCmdI28F) && data == kGetIdCmdI28F[index]);
    bool isErase =
        (index < sizeof(kEraseCmd28F) && data == kEraseCmd28F[index]) ||
        (index < sizeof(kEraseCmdAm28F) && data == kEraseCmdAm28F[index]) ||
        (index < sizeof(kEraseCmdI28F) && data == kEraseCmdI28F[index]);
    bool isReset =
        (index < sizeof(kResetCmd28F) && data == kResetCmd28F[index]) ||
        (index < sizeof(kResetCmdAm28F) && data == kResetCmdAm28F[index]);

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
    if (isVerify) {
        WriteToLog("Command: %02X (Verify)", data);
        f_operation = ChipOperationVerify;
        f_commandStep = 0xFF;
        return true;
    }
    if (isBlankCheck) {
        WriteToLog("Command: %02X (BlankCheck)", data);
        f_operation = ChipOperationBlankCheck;
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
        if (index == 0) {
            WriteToLog("Write Command (cycle #1): %02X", data);
            f_commandStep++;
            f_operation = ChipOperationErase;
        } else if (index == 1) {
            WriteToLog("Command: %02X (Erase)", data);
            // Erase
            WriteToLog("Erasing chip...");
            if (f_protected) {
                WriteToLog("Error! Device is protected (SDP enabled)");
            } else {
                FillData(0xFF);
            }
            f_commandStep = -1;
            f_operation = ChipOperationRead;
        }
        return true;
    }
    if (isReset) {
        if (index == 0) {
            WriteToLog("Write Command (cycle #1): %02X", data);
            f_commandStep++;
            f_operation = ChipOperationReset;
        } else if (index == 1) {
            WriteToLog("Command: %02X (Reset)", data);
            // Reset
            WriteToLog("Resetting command register...");
            f_operation = ChipOperationRead;
            f_commandStep = -1;
        }
        return true;
    }
    WriteToLog("Write Command: %02X (Invalid)", data);
    return false;
}
