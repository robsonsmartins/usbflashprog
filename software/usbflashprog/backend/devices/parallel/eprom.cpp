// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2023) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup Software
 * @file backend/devices/parallel/eprom.cpp
 * @brief Implementation of the Parallel EPROM.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QLoggingCategory>
#include "eprom.hpp"

// ---------------------------------------------------------------------------
// Logging

Q_LOGGING_CATEGORY(deviceParEprom, "device.parallel.eprom")

#define DEBUG qCDebug(deviceParEprom)
#define INFO qCInfo(deviceParEprom)
#define WARNING qCWarning(deviceParEprom)
#define CRITICAL qCCritical(deviceParEprom)
#define FATAL qCFatal(deviceParEprom)

// ---------------------------------------------------------------------------

EPROM::EPROM(QObject *parent) : ParDevice(parent) {
    info_.name = "EPROM";
    info_.capability.hasRead = true;
    info_.capability.hasProgram = true;
    info_.capability.hasVerify = true;
    info_.capability.hasBlankCheck = true;
    info_.capability.hasGetId = true;
    info_.capability.hasVDD = true;
    info_.capability.hasVPP = true;
    skipFF_ = true;
    twp_ = 600;
    twc_ = 8;
    vddRd_ = 5.0f;
    vddWr_ = 6.0f;
    vpp_ = 13.0f;
    vee_ = 13.0f;
    size_ = 2048;
    maxAttemptsProg_ = 25;
    flags_.skipFF = true;
    flags_.progWithVpp = true;
    flags_.pgmPositive = true;
    flags_.pgmCePin = true;
    algo_ = kCmdDeviceAlgorithmEPROM;
    DEBUG << info_.toString();
}

EPROM::~EPROM() {}

void EPROM::setSize(uint32_t value) {
    ParDevice::setSize(value);
    // set device flags
    switch (size_) {
        case 0x001000:  //  4KB
        case 0x010000:  // 64KB
        case 0x100000:  //  1MB
        case 0x400000:  //  4MB
            // ~OE/VPP Pin
            flags_.vppOePin = true;
            break;
        default:
            // ~OE and VPP Pin are separated
            flags_.vppOePin = false;
            break;
    }
    if (size_ == 0x800) {  // 2KB
        // Positive prog pulse (HI)
        flags_.pgmPositive = true;
    } else {
        // Normal negative prog pulse (LO)
        flags_.pgmPositive = false;
    }
    switch (size_) {
        case 0x02000:  //   8KB
        case 0x04000:  //  16KB
        case 0x20000:  // 128KB
        case 0x40000:  // 256KB
            // ~CE and ~PGM Pin are separated
            flags_.pgmCePin = false;
            break;
        default:
            // ~CE/PGM Pin
            flags_.pgmCePin = true;
            break;
    }
}

// ---------------------------------------------------------------------------

EPROM27::EPROM27(QObject *parent) : EPROM(parent) {
    info_.name = "EPROM 27 (NMOS)";
    twp_ = 50000;
    twc_ = 50;
    vddRd_ = 5.0f;
    vddWr_ = 5.0f;
    vpp_ = 25.0f;
    vee_ = 12.0f;
    DEBUG << info_.toString();
}

EPROM27::~EPROM27() {}

// ---------------------------------------------------------------------------

EPROM27C::EPROM27C(QObject *parent) : EPROM(parent) {
    info_.name = "EPROM 27C";
    twp_ = 500;
    twc_ = 8;
    vddRd_ = 5.0f;
    vddWr_ = 6.0f;
    vpp_ = 13.0f;
    vee_ = 12.0f;
    DEBUG << info_.toString();
}

EPROM27C::~EPROM27C() {}

// ---------------------------------------------------------------------------

EPROM27C16Bit::EPROM27C16Bit(QObject *parent) : EPROM27C(parent) {
    info_.name = "EPROM 27C (16Bit)";
    flags_.is16bit = true;
    DEBUG << info_.toString();
}

EPROM27C16Bit::~EPROM27C16Bit() {}

// ---------------------------------------------------------------------------

EPROM27E::EPROM27E(QObject *parent) : EPROM27C(parent) {
    info_.name = "EPROM W27C/27E/27SF";
    info_.capability.hasErase = true;
    info_.capability.hasFastProg = true;
    fastProg_ = true;
    twp_ = 100;
    twc_ = 15;
    vddRd_ = 5.0f;
    vddWr_ = 5.0f;
    vpp_ = 12.0f;
    vee_ = 14.0f;
    DEBUG << info_.toString();
}

EPROM27E::~EPROM27E() {}
