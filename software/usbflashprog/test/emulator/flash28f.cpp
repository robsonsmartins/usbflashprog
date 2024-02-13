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
 * @file test/emulator/flash28f.cpp
 * @brief Implementation of Flash 28F Chip Emulation.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <cstddef>
#include "flash28f.hpp"

// ---------------------------------------------------------------------------

/* @brief Hardcoded (emulated) Manufacturer ID: ST. */
constexpr uint8_t kChip28FManufacturerId = 0x20;
/* @brief Hardcoded (emulated) Device ID: M28F256. */
constexpr uint8_t kChip28FDeviceId = 0xA8;

/* @brief Hardcoded (emulated) Manufacturer ID: SST. */
constexpr uint8_t kChipSST28FManufacturerId = 0xBF;
/* @brief Hardcoded (emulated) Device ID: SST28SF/VF040A. */
constexpr uint8_t kChipSST28FDeviceId = 0x04;

/* @brief Hardcoded (emulated) Manufacturer ID: Intel. */
constexpr uint8_t kChipI28FManufacturerId = 0x89;
/* @brief Hardcoded (emulated) Device ID: 28F001BX-T. */
constexpr uint8_t kChipI28FDeviceId = 0x94;
/* @brief Hardcoded (emulated) Chip Status Byte. */
constexpr uint8_t kChipI28FStatusByte = 0x80;

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

/* SST28xF Chip commands. */
/* @brief SST 28xF Protect/Unprotect device sequence size. */
constexpr unsigned char kProtectSST28SFSize = 7;
/* @brief SST 28xF Unprotect device addresses. */
constexpr unsigned short kUnprotectSST28SF[] = { 
    0x1823, 0x1820, 0x1822, 0x0418, 0x041B, 0x0419, 0x041A 
};
/* @brief SST 28xF Protect device addresses. */
constexpr unsigned short kProtectSST28SF[] = { 
    0x1823, 0x1820, 0x1822, 0x0418, 0x041B, 0x0419, 0x040A
};

/* Intel 28F Chip commands. */
/* @brief Intel 28F Read command. */
constexpr uint8_t kReadCmdI28F  [] = { 0xFF };
/* @brief Intel 28F Write command. */
constexpr uint8_t kWriteCmdI28F [] = { 0x40 };
/* @brief Intel 28F Write 2 command. */
constexpr uint8_t kWrite2CmdI28F[] = { 0x10 };
/* @brief Intel 28F Erase device command. */
constexpr uint8_t kEraseCmdI28F [] = { 0x20, 0xD0 };
/* @brief Intel 28F GetID command. */
constexpr uint8_t kGetIdCmdI28F [] = { 0x90 };

// clang-format on

// ---------------------------------------------------------------------------

ChipFlash28F::ChipFlash28F()
    : BaseParChip(), f_commandStep(-1), f_operation(ChipOperationRead) {
    writeToLog("SetChip(%s)", "Flash 28F");
}

ChipFlash28F::~ChipFlash28F() {}

void ChipFlash28F::setSize(uint32_t size) {
    if (size == f_memory_area.size()) return;
    /* inherited */
    BaseParChip::setSize(size);
    /* initialize with 0xFF */
    fillData(0xFF);
    f_commandStep = -1;
    f_operation = ChipOperationRead;
}

void ChipFlash28F::emuChip(void) {
    bool isExecuteCmd = (f_commandStep == 0xFF);
    bool isGetId = (f_operation == ChipOperationGetId);
    bool isWrite = (f_operation == ChipOperationWrite);
    if (f_vdd && f_ce && !f_we && f_oe) {
        /* Read/GetID:
           VDD = 1; VPP = X; CE = 1; WE = 0; OE = 1; */
        if (isExecuteCmd && isGetId) {
            // GetID
            if (f_addr_bus == 0x00 && f_data_bus != kChip28FManufacturerId) {
                // return manufacturer ID
                f_data_bus = kChip28FManufacturerId;
                writeToLog("Manufacturer ID = %02X", f_data_bus);
            } else if (f_addr_bus == 0x01 && f_data_bus != kChip28FDeviceId) {
                // return device ID
                f_data_bus = kChip28FDeviceId;
                writeToLog("Device ID = %02X", f_data_bus);
                f_commandStep = -1;
                f_operation = ChipOperationRead;
            }
        } else {
            // Read
            read();
            f_commandStep = -1;
            f_operation = ChipOperationRead;
        }
    } else if (f_vdd && f_ce && f_we) {
        /* Command/Write/Erase:
           VDD = 1; VPP = X; CE = 1; WE = 1; OE = X; */
        if (isExecuteCmd && isWrite) {
            // Write
            write();
            f_commandStep = -1;
            f_operation = ChipOperationRead;
        } else {
            // Handle special commands
            if (!specialCommand()) write();
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

bool ChipFlash28F::specialCommand(void) {
    if (f_commandStep == 0xFF) f_commandStep = -1;
    int index = f_commandStep + 1;
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
        writeToLog("Command: %02X (Read)", data);
        f_operation = ChipOperationRead;
        f_commandStep = 0xFF;
        return true;
    }
    if (isWrite) {
        writeToLog("Command: %02X (Write)", data);
        f_operation = ChipOperationWrite;
        f_commandStep = 0xFF;
        return true;
    }
    if (isVerify) {
        writeToLog("Command: %02X (Verify)", data);
        f_operation = ChipOperationVerify;
        f_commandStep = 0xFF;
        return true;
    }
    if (isBlankCheck) {
        writeToLog("Command: %02X (BlankCheck)", data);
        f_operation = ChipOperationBlankCheck;
        f_commandStep = 0xFF;
        return true;
    }
    if (isGetId) {
        writeToLog("Command: %02X (GetID)", data);
        f_operation = ChipOperationGetId;
        f_commandStep = 0xFF;
        return true;
    }
    if (isErase) {
        if (index == 0) {
            writeToLog("Write Command (cycle #1): %02X", data);
            f_commandStep++;
            f_operation = ChipOperationErase;
        } else if (index == 1) {
            writeToLog("Command: %02X (Erase)", data);
            // Erase
            writeToLog("Erasing chip...");
            fillData(0xFF);
            f_commandStep = -1;
            f_operation = ChipOperationRead;
        }
        return true;
    }
    if (isReset) {
        if (index == 0) {
            writeToLog("Write Command (cycle #1): %02X", data);
            f_commandStep++;
            f_operation = ChipOperationReset;
        } else if (index == 1) {
            writeToLog("Command: %02X (Reset)", data);
            // Reset
            writeToLog("Resetting command register...");
            f_operation = ChipOperationRead;
            f_commandStep = -1;
        }
        return true;
    }
    writeToLog("Write Command: %02X (Invalid)", data);
    return false;
}

// ---------------------------------------------------------------------------

ChipFlashSST28F::ChipFlashSST28F()
    : BaseParChip(),
      f_commandStep(-1),
      f_protected(true),
      f_operation(ChipOperationRead) {
    writeToLog("SetChip(%s)", "Flash SST28xF");
}

ChipFlashSST28F::~ChipFlashSST28F() {}

void ChipFlashSST28F::setSize(uint32_t size) {
    if (size == f_memory_area.size()) return;
    /* inherited */
    BaseParChip::setSize(size);
    /* initialize with 0xFF */
    fillData(0xFF);
    f_commandStep = -1;
    f_operation = ChipOperationRead;
}

void ChipFlashSST28F::setVDD(bool state) {
    bool oldVdd = f_vdd;
    /* inherited */
    BaseParChip::setVDD(state);
    // SDP enable in poweron
    if (!oldVdd && state) f_protected = true;
}

void ChipFlashSST28F::read(void) {
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
        writeToLog("Unprotect (step #%d/%d)", index + 1, kProtectSST28SFSize);
        f_commandStep++;
        f_operation = ChipOperationUnprotect;
    } else if (f_addr_bus == kProtectSST28SF[index]) {
        writeToLog("Protect (step #%d/%d)", index + 1, kProtectSST28SFSize);
        f_commandStep++;
        f_operation = ChipOperationProtect;
    } else if (f_operation == ChipOperationUnprotect ||
               f_operation == ChipOperationProtect) {
        writeToLog("Cancel Protect/Unprotect sequence. Addr: %06X.",
                   f_addr_bus);
        f_commandStep = -1;
        f_operation = ChipOperationRead;
    }
    if ((f_commandStep + 1) >= kProtectSST28SFSize) {
        if (f_operation == ChipOperationProtect) {
            writeToLog("Protecting Device (Enabling SDP)");
        } else {
            writeToLog("Unprotecting Device (Disabling SDP)");
        }
        f_protected = (f_operation == ChipOperationProtect);
        f_commandStep = -1;
        f_operation = ChipOperationRead;
    }
    // returns the data from memory area
    f_data_bus = data;
    // update last_addr
    last_addr = f_addr_bus;
    writeToLog("Read(addr=%06.6lX) = %04.4X", f_addr_bus, f_data_bus);
}

void ChipFlashSST28F::emuChip(void) {
    bool isExecuteCmd = (f_commandStep == 0xFF);
    bool isGetId = (f_operation == ChipOperationGetId);
    bool isWrite = (f_operation == ChipOperationWrite);
    if (f_vdd && f_ce && !f_we && f_oe) {
        /* Read/GetID:
           VDD = 1; VPP = X; CE = 1; WE = 0; OE = 1; */
        if (isExecuteCmd && isGetId) {
            // GetID
            if (f_addr_bus == 0x00 && f_data_bus != kChipSST28FManufacturerId) {
                // return manufacturer ID
                f_data_bus = kChipSST28FManufacturerId;
                writeToLog("Manufacturer ID = %02X", f_data_bus);
            } else if (f_addr_bus == 0x01 &&
                       f_data_bus != kChipSST28FDeviceId) {
                // return device ID
                f_data_bus = kChipSST28FDeviceId;
                writeToLog("Device ID = %02X", f_data_bus);
                f_commandStep = -1;
                f_operation = ChipOperationRead;
            }
        } else {
            // Read
            read();
        }
    } else if (f_vdd && f_ce && f_we) {
        /* Command/Write/Erase:
           VDD = 1; VPP = X; CE = 1; WE = 1; OE = X; */
        if (isExecuteCmd && isWrite) {
            // Write
            if (f_protected) {
                writeToLog("Write Error! Device is protected (SDP enabled)");
            } else {
                write();
            }
            f_commandStep = -1;
            f_operation = ChipOperationRead;
        } else {
            // Handle special commands
            if (!specialCommand()) {
                if (f_protected) {
                    writeToLog(
                        "Write Error! Device is protected (SDP enabled)");
                } else {
                    write();
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

bool ChipFlashSST28F::specialCommand(void) {
    if (f_commandStep == 0xFF) f_commandStep = -1;
    unsigned int index = f_commandStep + 1;
    unsigned char data = f_data_bus & 0xFF;

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
        writeToLog("Command: %02X (Read)", data);
        f_operation = ChipOperationRead;
        f_commandStep = 0xFF;
        return true;
    }
    if (isWrite) {
        writeToLog("Command: %02X (Write)", data);
        f_operation = ChipOperationWrite;
        f_commandStep = 0xFF;
        return true;
    }
    if (isVerify) {
        writeToLog("Command: %02X (Verify)", data);
        f_operation = ChipOperationVerify;
        f_commandStep = 0xFF;
        return true;
    }
    if (isBlankCheck) {
        writeToLog("Command: %02X (BlankCheck)", data);
        f_operation = ChipOperationBlankCheck;
        f_commandStep = 0xFF;
        return true;
    }
    if (isGetId) {
        writeToLog("Command: %02X (GetID)", data);
        f_operation = ChipOperationGetId;
        f_commandStep = 0xFF;
        return true;
    }
    if (isErase) {
        if (index == 0) {
            writeToLog("Write Command (cycle #1): %02X", data);
            f_commandStep++;
            f_operation = ChipOperationErase;
        } else if (index == 1) {
            writeToLog("Command: %02X (Erase)", data);
            // Erase
            writeToLog("Erasing chip...");
            if (f_protected) {
                writeToLog("Error! Device is protected (SDP enabled)");
            } else {
                fillData(0xFF);
            }
            f_commandStep = -1;
            f_operation = ChipOperationRead;
        }
        return true;
    }
    if (isReset) {
        if (index == 0) {
            writeToLog("Write Command (cycle #1): %02X", data);
            f_commandStep++;
            f_operation = ChipOperationReset;
        } else if (index == 1) {
            writeToLog("Command: %02X (Reset)", data);
            // Reset
            writeToLog("Resetting command register...");
            f_operation = ChipOperationRead;
            f_commandStep = -1;
        }
        return true;
    }
    writeToLog("Write Command: %02X (Invalid)", data);
    return false;
}

// ---------------------------------------------------------------------------

ChipFlashIntel28F::ChipFlashIntel28F()
    : BaseParChip(), f_commandStep(-1), f_operation(ChipOperationRead) {
    writeToLog("SetChip(%s)", "Flash i28F");
}

ChipFlashIntel28F::~ChipFlashIntel28F() {}

void ChipFlashIntel28F::setSize(uint32_t size) {
    if (size == f_memory_area.size()) return;
    /* inherited */
    BaseParChip::setSize(size);
    /* initialize with 0xFF */
    fillData(0xFFFF);
    f_commandStep = -1;
    f_operation = ChipOperationRead;
}

uint16_t ChipFlashIntel28F::getDataBus(void) {
    // updates the data bus
    updateDataBus(false);
    // returns the data bus
    return f_data_bus;
}

bool ChipFlashIntel28F::updateDataBus(bool readMemory) {
    bool isExecuteCmd = (f_commandStep == 0xFF);
    bool isGetId = (f_operation == ChipOperationGetId);
    bool isWrite = (f_operation == ChipOperationWrite);
    bool isErase = (f_operation == ChipOperationErase);
    if (f_vdd && f_ce && !f_we && f_oe) {
        /* Read/GetID/Read StatusByte:
           VDD = 1; VPP = X; CE = 1; WE = 0; OE = 1; */
        if (isExecuteCmd && isGetId) {
            // GetID
            if (f_addr_bus == 0x00 && f_data_bus != kChipI28FManufacturerId) {
                // return manufacturer ID
                f_data_bus = kChipI28FManufacturerId;
                writeToLog("Manufacturer ID = %02X", f_data_bus);
            } else if (f_addr_bus == 0x01 && f_data_bus != kChipI28FDeviceId) {
                // reutrn device ID
                f_data_bus = kChipI28FDeviceId;
                writeToLog("Device ID = %02X", f_data_bus);
                return true;
            }
        } else if (isExecuteCmd && (isWrite || isErase)) {
            // Write or Erase
            // return Status Byte
            f_data_bus = kChipI28FStatusByte;
            writeToLog("Status Byte = %02X", f_data_bus);
            return true;
        } else if (readMemory) {
            // Read
            read();
            return true;
        }
    }
    return false;
}

void ChipFlashIntel28F::emuChip(void) {
    bool isExecuteCmd = (f_commandStep == 0xFF);
    bool isWrite = (f_operation == ChipOperationWrite);
    if (f_vdd && f_ce && !f_we && f_oe) {
        /* Read/GetID/Read StatusByte:
           VDD = 1; VPP = X; CE = 1; WE = 0; OE = 1; */
        // Update DataBus/Read
        if (updateDataBus()) {
            f_commandStep = -1;
            f_operation = ChipOperationRead;
        }
    } else if (f_vdd && f_ce && f_we) {
        /* Command/Write/Erase:
           VDD = 1; VPP = X; CE = 1; WE = 1; OE = X; */
        if (isExecuteCmd && isWrite) {
            // Write
            write();
        } else {
            // Handle special commands
            if (!specialCommand()) write();
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

bool ChipFlashIntel28F::specialCommand(void) {
    if (f_commandStep == 0xFF) f_commandStep = -1;
    int index = f_commandStep + 1;
    uint8_t data = f_data_bus & 0xFF;

    bool isRead = (index < sizeof(kReadCmdI28F) && data == kReadCmdI28F[index]);
    bool isWrite =
        (index < sizeof(kWriteCmdI28F) && data == kWriteCmdI28F[index]);
    bool isGetId =
        (index < sizeof(kGetIdCmdI28F) && data == kGetIdCmdI28F[index]);
    bool isErase =
        (index < sizeof(kEraseCmdI28F) && data == kEraseCmdI28F[index]);

    if (isRead) {
        writeToLog("Command: %02X (Read)", data);
        f_operation = ChipOperationRead;
        f_commandStep = 0xFF;
        return true;
    }
    if (isWrite) {
        writeToLog("Command: %02X (Write)", data);
        f_operation = ChipOperationWrite;
        f_commandStep = 0xFF;
        return true;
    }
    if (isGetId) {
        writeToLog("Command: %02X (GetID)", data);
        f_operation = ChipOperationGetId;
        f_commandStep = 0xFF;
        return true;
    }
    if (isErase) {
        f_operation = ChipOperationErase;
        if (index == 0) {
            writeToLog("Write Command (cycle #1): %02X", data);
            f_commandStep++;
        } else if (index == 1) {
            writeToLog("Command: %02X (Erase)", data);
            // Erase chip
            writeToLog("Erasing chip...");
            fillData(0xFFFF);
            f_commandStep = 0xFF;
        }
        return true;
    }
    writeToLog("Write Command: %02X (Invalid)", data);
    return false;
}
