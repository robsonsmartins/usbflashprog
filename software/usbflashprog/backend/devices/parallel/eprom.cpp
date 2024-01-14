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

bool EPROM::read(QByteArray &buffer) {
    INFO << "Reading device...";
    uint32_t total = size_;
    if (is16bit_) total /= 2;
    canceling_ = false;
    // Init pins/bus to Read operation
    if (!initDevice(kDeviceOpRead)) {
        WARNING << "Error reading device";
        return false;
    }
    bool error = false;
    // Read the device
    if (!readDevice(buffer)) error = true;
    if (!error) emit onProgress(total, total, true);
    // Close resources
    finalizeDevice();
    if (error) {
        WARNING << "Error reading device";
    } else {
        INFO << "Reading device OK";
    }
    return !error;
}

bool EPROM::program(const QByteArray &buffer, bool verify) {
    INFO << "Programming device...";
    uint32_t total = qMin(size_, static_cast<uint32_t>(buffer.size()));
    if (is16bit_) total /= 2;
    canceling_ = false;
    // Init pins/bus to Prog operation
    if (!initDevice(kDeviceOpProg)) {
        return false;
        WARNING << "Error programming device";
    }
    bool error = false;
    // Program the device
    if (!programDevice(buffer)) error = true;
    // Close resources
    finalizeDevice();
    // If error, returns
    if (error) {
        WARNING << "Error programming device";
        return false;
    }
    // If no error and verify flag is disabled, return
    if (!verify) {
        emit onProgress(total, total, true);
        INFO << "Programming device OK";
        return true;
    }
    // If verify flag is enabled, verify device
    return this->verify(buffer);
}

bool EPROM::verify(const QByteArray &buffer) {
    INFO << "Verifying device...";
    uint32_t total = qMin(size_, static_cast<uint32_t>(buffer.size()));
    if (is16bit_) total /= 2;
    canceling_ = false;
    // Init pins/bus to Read operation
    if (!initDevice(kDeviceOpRead)) {
        WARNING << "Error verifying device";
        return false;
    }
    bool error = false;
    // Read the device and verify (compare) buffer
    if (!verifyDevice(buffer)) error = true;
    if (!error) emit onProgress(total, total, true);
    // Close resources
    finalizeDevice();
    if (error) {
        WARNING << "Error verifying device";
    } else {
        INFO << "Verifying device OK";
    }
    return !error;
}

bool EPROM::blankCheck() {
    // Create a blank (0xFF) buffer
    QByteArray data = QByteArray(size_, (char)0xFF);
    // Verify
    return verify(data);
}

bool EPROM::getId(TDeviceID &result) {
    INFO << "Getting ID from device...";
    uint32_t total = size_;
    if (is16bit_) total /= 2;
    canceling_ = false;
    // Init pins/bus to GetId operation
    if (!initDevice(kDeviceOpGetId)) {
        WARNING << "Error getting ID from device";
        return false;
    }
    bool error = false;
    // Get manufacturer data (byte)
    result.manufacturer = runner_.dataGet();
    if (runner_.hasError()) error = true;
    // Increment Address (0x01)
    runner_.addrInc();
    // Get device data (byte)
    result.device = runner_.dataGet();
    // Error
    if (runner_.hasError()) {
        WARNING << "Error getting ID from device";
        return finalizeDevice(0, total, true, false);
    }
    // Success
    if (!error) emit onProgress(total, total, true);
    // Close resources
    finalizeDevice();
    if (error) {
        WARNING << "Error getting ID from device";
    } else {
        INFO << "Getting ID from device OK";
    }
    return (!error);
}

// ---------------------------------------------------------------------------

EPROM27::EPROM27(QObject *parent) : EPROM(parent) {
    info_.name = "EPROM 27xxx";
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
    info_.name = "EPROM 27Cxxx";
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
    info_.name = "EPROM 27Cxxx (16Bit)";
    is16bit_ = true;
    DEBUG << info_.toString();
}

EPROM27C16Bit::~EPROM27C16Bit() {}

// ---------------------------------------------------------------------------

EPROM27E::EPROM27E(QObject *parent) : EPROM27C(parent) {
    info_.name = "EPROM W27C/27E/27SF";
    info_.capability.hasErase = true;
    twp_ = 100;
    twc_ = 15;
    vddRd_ = 5.0f;
    vddWr_ = 5.0f;
    vpp_ = 12.0f;
    vee_ = 14.0f;
    erasePulseDelay_ = 100;  // 100 ms
    DEBUG << info_.toString();
}

EPROM27E::~EPROM27E() {}

bool EPROM27E::erase(bool check) {
    INFO << "Erasing device...";
    uint32_t total = size_;
    if (is16bit_) total /= 2;
    canceling_ = false;
    // Init pins/bus to Prog operation
    if (!initDevice(kDeviceOpErase)) {
        WARNING << "Error erasing device";
        return false;
    }
    bool error = false;
    // Erase the device
    if (!eraseDevice()) error = true;
    // Close resources
    finalizeDevice();
    // If error, returns
    if (error) {
        WARNING << "Error erasing device";
        return false;
    }
    // If no error and check flag is disabled, return
    if (!check) {
        emit onProgress(total, total, true);
        INFO << "Erasing device OK";
        return true;
    }
    // If check flag is enabled, verify device
    return blankCheck();
}

bool EPROM27E::initDevice(kDeviceOperation operation) {
    if (operation != kDeviceOpErase) {
        return EPROM27C::initDevice(operation);
    }
    // Operation == Erase
    DEBUG << "Initializing...";
    if (!runner_.open(port_)) {
        emit onProgress(0, size_, true, false);
        WARNING << "Error opening Serial Port";
        return false;
    }
    // Init bus and pins
    resetBus_();
    // VDD Wr on
    runner_.vddSet(vddWr_);
    runner_.vddCtrl();
    initControlPins_(false);
    // VPP set with VEE
    // VPP on
    runner_.vppSet(vee_);
    runner_.vppCtrl();
    // VPP on A9
    runner_.vppOnA9();
    // Data is 0xFF
    if (is16bit_) {
        runner_.dataSetW(0xFFFF);
    } else {
        runner_.dataSet(0xFF);
    }
    runner_.usDelay(initDelay_);
    if (runner_.hasError()) return finalizeDevice(0, size_, true, false);
    DEBUG << "Initialize OK";
    return true;
}

bool EPROM27E::eraseDevice() {
    DEBUG << "Erasing data...";
    uint32_t current = 0;
    uint32_t total = size_;
    if (is16bit_) total /= 2;
    int increment = is16bit_ ? 2 : 1;
    uint16_t read = 0xFFFF;
    uint16_t empty = is16bit_ ? 0xFFFF : 0xFF;
    // Repeat for n max attempts
    for (int j = 1; j <= maxAttemptsProg_; j++) {
        // Erase entire chip
        // Addr = 0
        runner_.addrClr();
        // Data = 0xFF
        if (is16bit_) {
            runner_.dataSetW(0xFFFF);
        } else {
            runner_.dataSet(0xFF);
        }
        // VPP on A9
        runner_.vppOnA9();
        if (flags_.pgmPositive) {
            // PGM is HI (start erase pulse)
            runner_.setWE(false);
        } else {
            // ~PGM is LO (start erase pulse)
            runner_.setWE();
        }
        runner_.msDelay(erasePulseDelay_);
        if (flags_.pgmPositive) {
            // PGM is LO (end erase pulse)
            runner_.setWE();
        } else {
            // ~PGM is HI (end erase pulse)
            runner_.setWE(false);
        }
        runner_.usDelay(twc_);
        // VPP on A9 off
        runner_.vppOnA9(false);
        // PGM/~CE is LO
        if (flags_.pgmCePin) runner_.setWE();
        bool success = true;
        for (current = 0; current < total; current++) {
            if (canceling_) {
                emit onProgress(current, total, true, false, true);
                DEBUG << QString("Erase canceled at 0x%1 of 0x%2")
                             .arg(current, 6, 16, QChar('0'))
                             .arg(total, 6, 16, QChar('0'));
                return false;
            }
            // Verify data
            if (!verifyData_(empty)) {
                success = false;
                break;
            }
            if (runner_.hasError() || j == maxAttemptsProg_) {
                // Error
                emit onProgress(current, total, true, false);
                WARNING << QString("Erase error at 0x%1 of 0x%2")
                               .arg(current, 6, 16, QChar('0'))
                               .arg(total, 6, 16, QChar('0'));
                if (runner_.hasError()) {
                    WARNING << "Runner status is Error";
                } else {
                    WARNING << QString("Data to write 0x%1. Data read 0x%2")
                                   .arg(0xFF, 2, 16, QChar('0'))
                                   .arg(read, 2, 16, QChar('0'));
                }
                return false;
            }
        }
        if (success) break;
    }
    DEBUG << "Erase OK";
    return true;
}
