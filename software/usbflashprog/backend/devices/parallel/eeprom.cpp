// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2024) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup Software
 * @file backend/devices/parallel/eeprom.cpp
 * @brief Implementation of the Parallel EEPROM.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QLoggingCategory>
#include "eeprom.hpp"

// ---------------------------------------------------------------------------
// Logging

Q_LOGGING_CATEGORY(deviceParEEprom, "device.parallel.eeprom")

#define DEBUG qCDebug(deviceParEEprom)
#define INFO qCInfo(deviceParEEprom)
#define WARNING qCWarning(deviceParEEprom)
#define CRITICAL qCCritical(deviceParEEprom)
#define FATAL qCFatal(deviceParEEprom)

// ---------------------------------------------------------------------------

EEPROM::EEPROM(QObject *parent) : ParDevice(parent) {
    info_.name = "EEPROM";
    info_.capability.hasRead = true;
    info_.capability.hasProgram = true;
    info_.capability.hasVerify = true;
    info_.capability.hasBlankCheck = true;
    info_.capability.hasVDD = true;
    info_.capability.hasErase = true;
    vddRd_ = 5.0f;
    vddWr_ = 5.0f;
    vpp_ = 12.0f;
    vee_ = 12.0f;
    size_ = 2048;
    twp_ = 2;
    twc_ = 10000;
    protectAlgo_ = kCmdDeviceAlgorithm28C64;
    maxAttemptsProg_ = 3;
    DEBUG << info_.toString();
}

EEPROM::~EEPROM() {}

void EEPROM::setSize(uint32_t value) {
    ParDevice::setSize(value);
    protectAlgo_ = (size_ <= 0x2000) ? kCmdDeviceAlgorithm28C64
                                     : kCmdDeviceAlgorithm28C256;
    bool oldValue = info_.capability.hasUnprotect;
    bool newValue = (size_ >= 0x2000);  // >= 8KB
    if (oldValue != newValue) {
        info_.capability.hasUnprotect = newValue;
        info_.capability.hasProtect = newValue;
        DEBUG << info_.toString();
    }
}

bool EEPROM::eraseDevice() {
    DEBUG << "Erasing data...";
    // Create a blank (0xFF) buffer
    QByteArray data = QByteArray(size_, (char)0xFF);
    if (!programDevice(data)) {
        WARNING << "Erase error";
        return false;
    }
    DEBUG << "Erase OK";
    return true;
}

// ---------------------------------------------------------------------------

EEPROM28C::EEPROM28C(QObject *parent) : EEPROM(parent) {
    info_.name = "EEPROM 28C/X28";
    DEBUG << info_.toString();
}

EEPROM28C::~EEPROM28C() {}

// ---------------------------------------------------------------------------

EEPROM28AT::EEPROM28AT(QObject *parent) : EEPROM(parent) {
    info_.name = "EEPROM AT28C";
    DEBUG << info_.toString();
}

EEPROM28AT::~EEPROM28AT() {}

void EEPROM28AT::setSize(uint32_t value) {
    EEPROM::setSize(value);
    switch (size_) {
        case 0x02000:  //   8KB
        case 0x04000:  //  16KB
        case 0x08000:  //  32KB
            sectorSize_ = 64;
            break;
        case 0x10000:  //  64KB
        case 0x20000:  // 128KB
            sectorSize_ = 128;
            break;
        case 0x40000:  // 256KB
            sectorSize_ = 256;
            break;
        default:
            if (size_ < 0x02000) sectorSize_ = 0;
            if (size_ > 0x40000) sectorSize_ = 256;
            break;
    }
    bool oldValue = info_.capability.hasSectorSize;
    bool newValue = (size_ >= 0x2000);  // >= 8KB
    if (oldValue != newValue) {
        info_.capability.hasSectorSize = newValue;
        DEBUG << info_.toString();
    }
}
