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
 * @file backend/devices/parallel/flash28f.cpp
 * @brief Implementation of the Parallel Flash 28F.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QLoggingCategory>
#include "flash28f.hpp"

// ---------------------------------------------------------------------------
// Logging

Q_LOGGING_CATEGORY(deviceParFlash28F, "device.parallel.flash28f")

#define DEBUG qCDebug(deviceParFlash28F)
#define INFO qCInfo(deviceParFlash28F)
#define WARNING qCWarning(deviceParFlash28F)
#define CRITICAL qCCritical(deviceParFlash28F)
#define FATAL qCFatal(deviceParFlash28F)

// ---------------------------------------------------------------------------

Flash28F::Flash28F(QObject *parent) : ParDevice(parent) {
    info_.name = "Flash 28F";
    info_.capability.hasRead = true;
    info_.capability.hasProgram = true;
    info_.capability.hasVerify = true;
    info_.capability.hasBlankCheck = true;
    info_.capability.hasVDD = true;
    info_.capability.hasVPP = true;
    info_.capability.hasErase = true;
    info_.capability.hasGetId = true;
    vddRd_ = 5.0f;
    vddWr_ = 5.0f;
    vpp_ = 12.0f;
    vee_ = 12.0f;
    size_ = 2048;
    twp_ = 20;
    twc_ = 30;
    maxAttemptsProg_ = 3;
    flags_.progWithVpp = true;
    algo_ = kCmdDeviceAlgorithmFlash28F;
    DEBUG << info_.toString();
}

Flash28F::~Flash28F() {}

bool Flash28F::eraseDevice() {
    DEBUG << "Erasing data...";
    // Create a zeroes (0x00) buffer
    QByteArray data = QByteArray(size_, (char)0x00);
    // program entire device with 0x00
    if (!programDevice(data)) {
        WARNING << "Erase error";
        return false;
    }
    // call default erase routine
    return ParDevice::eraseDevice();
}

// ---------------------------------------------------------------------------

FlashSST28SF::FlashSST28SF(QObject *parent) : Flash28F(parent) {
    info_.capability.hasVPP = false;
    twp_ = 7;
    twc_ = 50;
    flags_.progWithVpp = false;
    algo_ = kCmdDeviceAlgorithmFlashSST28SF;
    DEBUG << info_.toString();
}

FlashSST28SF::~FlashSST28SF() {}

// ---------------------------------------------------------------------------

FlashAm28F::FlashAm28F(QObject *parent) : Flash28F(parent) {
    twp_ = 20;
    twc_ = 30;
    algo_ = kCmdDeviceAlgorithmFlashAm28F;
    DEBUG << info_.toString();
}

FlashAm28F::~FlashAm28F() {}

// ---------------------------------------------------------------------------

FlashI28F::FlashI28F(QObject *parent) : Flash28F(parent) {
    twp_ = 3;
    twc_ = 20;
    algo_ = kCmdDeviceAlgorithmFlashI28F;
    DEBUG << info_.toString();
}

FlashI28F::~FlashI28F() {}

// ---------------------------------------------------------------------------

FlashSharpI28F::FlashSharpI28F(QObject *parent) : FlashI28F(parent) {
    info_.capability.hasVPP = false;
    flags_.progWithVpp = false;
    DEBUG << info_.toString();
}

FlashSharpI28F::~FlashSharpI28F() {}

// ---------------------------------------------------------------------------

FlashI28F16Bit::FlashI28F16Bit(QObject *parent) : FlashI28F(parent) {
    flags_.is16bit = true;
    DEBUG << info_.toString();
}

FlashI28F16Bit::~FlashI28F16Bit() {}
